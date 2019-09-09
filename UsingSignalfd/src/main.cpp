
#include <cstdio>
#include <initializer_list>

#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>

#include "kbase/error_exception_util.h"
#include "kbase/scope_guard.h"

#include "ezio/event_loop.h"
#include "ezio/notifier.h"
#include "ezio/this_thread.h"
#include "ezio/thread.h"

int MakeSignalfd(std::initializer_list<int> signals)
{
    sigset_t mask;
    sigemptyset(&mask);
    for (auto sig : signals) {
        sigaddset(&mask, sig);
    }

    int rv = pthread_sigmask(SIG_BLOCK, &mask, nullptr);
    ENSURE(CHECK, rv != -1)(errno).Require("pthread_sigmask failure");

    auto fd = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
    ENSURE(CHECK, fd > 0)(fd)(errno).Require("signalfd failure");

    return fd;
}

ezio::EventLoop* g_loop = nullptr;

int main()
{
    ezio::EventLoop main_loop;
    g_loop = &main_loop;

    // Block signals and create a signalfd.
    ezio::ScopedSocket sigfd(MakeSignalfd({SIGINT, SIGALRM}));

    ezio::Notifier signal_notifier(&main_loop, sigfd);
    signal_notifier.set_on_read([&sigfd](ezio::TimePoint, ezio::IOContext::Details) {
        struct signalfd_siginfo sig_info {};
        auto len = read(sigfd.get(), &sig_info, sizeof(sig_info));
        ENSURE(THROW, len == sizeof(sig_info))(len)(errno).Require();
        if (sig_info.ssi_signo == SIGINT) {
            printf("[tid=%u] Receive SIGINT; quit now\n", ezio::this_thread::GetID());
            g_loop->Quit();
        } else if (sig_info.ssi_signo == SIGALRM) {
            printf("[tid=%u] timer Duang~Duang~Duang\n", ezio::this_thread::GetID());
        } else {
            ENSURE(THROW, kbase::NotReached())(sig_info.ssi_signo).Require();
        }
    });

    signal_notifier.EnableReading();

    printf("[tid=%u]main loop starts running\n", ezio::this_thread::GetID());

    // Worker thread
    ezio::Thread th("worker");
    th.event_loop()->RunTask([] {
        printf("[tid=%u]worker thread starts running\n", ezio::this_thread::GetID());
        alarm(5);
    });

    main_loop.RunTask([] {
        alarm(7);
    });

    main_loop.Run();

    return 0;
}
