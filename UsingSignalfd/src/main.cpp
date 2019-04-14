
#include <cstdio>
#include <initializer_list>

#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>

#include "kbase/error_exception_util.h"

#include "ezio/event_loop.h"
#include "ezio/notifier.h"

int NewSignalFDForSignals(std::initializer_list<int> signals)
{
    sigset_t mask;
    sigemptyset(&mask);
    for (auto sig : signals) {
        sigaddset(&mask, sig);
    }

    int rv = sigprocmask(SIG_BLOCK, &mask, nullptr);
    ENSURE(CHECK, rv != -1)(errno).Require();

    auto fd = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
    ENSURE(CHECK, fd > 0)(fd)(errno).Require();

    return fd;
}

ezio::EventLoop* g_loop = nullptr;

int main()
{
    ezio::EventLoop main_loop;
    g_loop = &main_loop;

    ezio::ScopedSocket sigfd(NewSignalFDForSignals({SIGINT, SIGALRM}));

    ezio::Notifier signal_notifier(&main_loop, sigfd);
    signal_notifier.set_on_read([&sigfd](ezio::TimePoint, ezio::IOContext::Details) {
        struct signalfd_siginfo sig_info {};
        auto len = read(sigfd.get(), &sig_info, sizeof(sig_info));
        ENSURE(THROW, len == sizeof(sig_info))(len)(errno).Require();
        if (sig_info.ssi_signo == SIGINT) {
            printf("Receive SIGINT; quit now\n");
            g_loop->Quit();
        } else if (sig_info.ssi_signo == SIGALRM) {
            printf("Duang~Duang~Duang\n");
        } else {
            ENSURE(THROW, kbase::NotReached())(sig_info.ssi_signo).Require();
        }
    });

    signal_notifier.EnableReading();

    // expire in 3 seconds.
    alarm(3);

    printf("main loop starts running\n");

    main_loop.Run();

    return 0;
}
