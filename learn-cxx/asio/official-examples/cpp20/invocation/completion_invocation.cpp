//
// completion_executor.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <concepts>
#include <iostream>

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

// The logging_executor class implements a minimal executor that satisfies the
// requirements for use as a completion executor. This means it may be bound to
// a handler using bind_executor.
class logging_executor {
public:
    // All executors must be no-throw equality comparable.
    bool operator==(const logging_executor&) const noexcept = default;

    // All executors must provide a const member function execute().
    void execute(std::invocable auto handler) const {
        try {
            std::cout << "handler invocation starting\n";
            std::move(handler)();
            std::cout << "handler invocation complete\n";
        } catch (...) {
            std::cout << "handler invocation completed with exception\n";
            throw;
        }
    }
};

// Confirm that a logging_executor satisfies the executor requirements.
static_assert(boost::asio::execution::executor<logging_executor>);

// Confirm that a logging_executor can be used as a completion executor.
static_assert(std::convertible_to<
              logging_executor, boost::asio::any_completion_executor>);

//----------------------------------------------------------------------

int main() {
    boost::asio::io_context io_context(1);

    // Post a completion handler to be run immediately.
    boost::asio::post(io_context,
                      boost::asio::bind_executor(logging_executor{},
                                                 [] { std::cout << "post complete\n"; }));

    // Start an asynchronous accept that will complete immediately.
    tcp::endpoint endpoint(boost::asio::ip::address_v4::loopback(), 0);
    tcp::acceptor acceptor(io_context, endpoint);
    tcp::socket server_socket(io_context);
    acceptor.async_accept(
            boost::asio::bind_executor(logging_executor{},
                                       [](auto...) { std::cout << "async_accept complete\n"; }));
    tcp::socket client_socket(io_context);
    client_socket.connect(acceptor.local_endpoint());

    // Set a timer to expire immediately.
    boost::asio::steady_timer timer(io_context);
    timer.expires_at(boost::asio::steady_timer::clock_type::time_point::min());
    timer.async_wait(
            boost::asio::bind_executor(logging_executor{},
                                       [](auto) { std::cout << "async_wait complete\n"; }));

    io_context.run();

    return 0;
}
