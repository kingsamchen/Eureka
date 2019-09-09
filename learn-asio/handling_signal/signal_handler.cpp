/*
 @ 0xCCCCCCCC
*/

#include <cstdio>
#include <functional>

#include "asio/executor_work_guard.hpp"
#include "asio/io_context.hpp"
#include "asio/signal_set.hpp"

int main()
{
    asio::io_context ioc;

    asio::signal_set signals(ioc, SIGINT, SIGTERM, SIGALRM);

    std::function<void()> p = [&] {
        signals.async_wait([&ioc, &p](std::error_code ec, int sig_no) {
            printf("Recv signal=%d\n", sig_no);
            if (sig_no == SIGINT || sig_no == SIGTERM) {
                ioc.stop();
                printf("Quit\n");
            }
            p();
        });
    };
    p();

    printf("Running\n");

    auto guard = asio::make_work_guard(ioc);
    ioc.run();

    printf("main thread exit\n");

    return 0;
}