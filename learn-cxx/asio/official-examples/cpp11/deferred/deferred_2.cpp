//
// deferred_2.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <chrono>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <iostream>

using boost::asio::deferred;

// NOTE: this example is not a good introduction to deferred, as lazily execute `async_wait()`
// won't postpone the expiration, thus its kind of misleading.

int main() {
    boost::asio::io_context ctx;

    boost::asio::steady_timer timer(ctx);
    timer.expires_after(std::chrono::seconds(2));

    fmt::println("[{}] Set up timer expiration", std::chrono::system_clock::now());

    auto deferred_op = timer.async_wait(
            deferred(
                    [&](boost::system::error_code ec) {
                        fmt::println("[{}] first timer wait finished: {}",
                                     std::chrono::system_clock::now(), ec.message());
                        timer.expires_after(std::chrono::seconds(2));
                        return timer.async_wait(deferred);
                    }));

    fmt::println("[{}] Deferred with first async_wait", std::chrono::system_clock::now());

    // Simulate busy work.
    std::this_thread::sleep_for(std::chrono::seconds{1});

    fmt::println("[{}] Launch deferred op", std::chrono::system_clock::now());

    std::move(deferred_op)(
            [](boost::system::error_code ec) {
                fmt::println("[{}] second timer wait finished: {}",
                             std::chrono::system_clock::now(), ec.message());
            });

    ctx.run();

    return 0;
}
