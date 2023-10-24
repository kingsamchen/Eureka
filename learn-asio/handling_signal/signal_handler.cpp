/*
 @ 0xCCCCCCCC
*/

#include <cstdio>
#include <functional>

#include "asio/executor_work_guard.hpp"
#include "asio/io_context.hpp"
#include "asio/signal_set.hpp"
#include "fmt/format.h"

int main() {
    asio::io_context ioc;

    asio::signal_set signals(ioc, SIGINT, SIGTERM);

    std::function<void()> p = [&] {
        signals.async_wait([&ioc](std::error_code, int signo) {
            fmt::println("Recv signal={}", signo);
            ioc.stop();
            fmt::println("Quit");
        });
    };
    p();

    fmt::println("Running");

    auto guard = asio::make_work_guard(ioc);
    ioc.run();

    fmt::println("main thread exit");

    return 0;
}
