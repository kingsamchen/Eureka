//
// composed_7.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio/compose.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

using boost::asio::ip::tcp;

// NOTE: This example requires the new boost::asio::async_compose function. For
// an example that works with the Networking TS style of completion tokens,
// please see an older version of asio.

//------------------------------------------------------------------------------

// This composed operation shows composition of multiple underlying operations.
// It automatically serialises a message, using its I/O streams insertion
// operator, before sending it N times on the socket. To do this, it must
// allocate a buffer for the encoded message and ensure this buffer's validity
// until all underlying async_write operation complete. A one second delay is
// inserted prior to each write operation, using a steady_timer.

template<typename T, typename CompletionToken>
auto async_write_messages(tcp::socket& socket,
                          const T& message, std::size_t repeat_count,
                          CompletionToken&& token)
// The return type of the initiating function is deduced from the combination
// of:
//
// - the CompletionToken type,
// - the completion handler signature, and
// - the asynchronous operation's initiation function object.
//
// When the completion token is a simple callback, the return type is always
// void. In this example, when the completion token is boost::asio::yield_context
// (used for stackful coroutines) the return type would also be void, as
// there is no non-error argument to the completion handler. When the
// completion token is boost::asio::use_future it would be std::future<void>. When
// the completion token is boost::asio::deferred, the return type differs for each
// asynchronous operation.
//
// In C++14 we can omit the return type as it is automatically deduced from
// the return type of boost::asio::async_compose.
{
    // Encode the message and copy it into an allocated buffer. The buffer will
    // be maintained for the lifetime of the composed asynchronous operation.
    std::ostringstream os;
    os << message;
    std::unique_ptr<std::string> encoded_message(new std::string(os.str()));

    // Create a steady_timer to be used for the delay between messages.
    std::unique_ptr<boost::asio::steady_timer> delay_timer(
            new boost::asio::steady_timer(socket.get_executor()));

    // To manage the cycle between the multiple underlying asynchronous
    // operations, our implementation is a state machine.
    enum { starting,
           waiting,
           writing };

    // The boost::asio::async_compose function takes:
    //
    // - our asynchronous operation implementation,
    // - the completion token,
    // - the completion handler signature, and
    // - any I/O objects (or executors) used by the operation
    //
    // It then wraps our implementation, which is implemented here as a state
    // machine in a lambda, in an intermediate completion handler that meets the
    // requirements of a conforming asynchronous operation. This includes
    // tracking outstanding work against the I/O executors associated with the
    // operation (in this example, this is the socket's executor).
    //
    // The first argument to our lambda is a reference to the enclosing
    // intermediate completion handler. This intermediate completion handler is
    // provided for us by the boost::asio::async_compose function, and takes care
    // of all the details required to implement a conforming asynchronous
    // operation. When calling an underlying asynchronous operation, we pass it
    // this enclosing intermediate completion handler as the completion token.
    //
    // All arguments to our lambda after the first must be defaulted to allow the
    // state machine to be started, as well as to allow the completion handler to
    // match the completion signature of both the async_write and
    // steady_timer::async_wait operations.
    return boost::asio::async_compose<
            CompletionToken, void(boost::system::error_code)>(
            [
                    // The implementation holds a reference to the socket as it is used for
                    // multiple async_write operations.
                    &socket,

                    // The allocated buffer for the encoded message. The std::unique_ptr
                    // smart pointer is move-only, and as a consequence our lambda
                    // implementation is also move-only.
                    encoded_message = std::move(encoded_message),

                    // The repeat count remaining.
                    repeat_count,

                    // A steady timer used for introducing a delay.
                    delay_timer = std::move(delay_timer),

                    // To manage the cycle between the multiple underlying asynchronous
                    // operations, our implementation is a state machine.
                    state = starting](
                    auto& self,
                    const boost::system::error_code& error = {},
                    std::size_t = 0) mutable {
                if (!error) {
                    switch (state) {
                    case starting:
                    case writing:
                        if (repeat_count > 0) {
                            --repeat_count;
                            state = waiting;
                            delay_timer->expires_after(std::chrono::seconds(1));
                            delay_timer->async_wait(std::move(self));
                            return; // Composed operation not yet complete.
                        }
                        break; // Composed operation complete, continue below.
                    case waiting:
                        state = writing;
                        boost::asio::async_write(socket,
                                                 boost::asio::buffer(*encoded_message), std::move(self));
                        return; // Composed operation not yet complete.
                    }
                }

                // This point is reached only on completion of the entire composed
                // operation.

                // Deallocate the encoded message and delay timer before calling the
                // user-supplied completion handler.
                encoded_message.reset();
                delay_timer.reset();

                // Call the user-supplied handler with the result of the operation.
                self.complete(error);
            },
            token, socket);
}

//------------------------------------------------------------------------------

void test_callback() {
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, {tcp::v4(), 55555});
    tcp::socket socket = acceptor.accept();

    // Test our asynchronous operation using a lambda as a callback.
    async_write_messages(socket, "Testing callback\r\n", 5,
                         [](const boost::system::error_code& error) {
                             if (!error) {
                                 std::cout << "Messages sent\n";
                             } else {
                                 std::cout << "Error: " << error.message() << "\n";
                             }
                         });

    io_context.run();
}

//------------------------------------------------------------------------------

void test_deferred() {
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, {tcp::v4(), 55555});
    tcp::socket socket = acceptor.accept();

    // Test our asynchronous operation using the deferred completion token. This
    // token causes the operation's initiating function to package up the
    // operation with its arguments to return a function object, which may then be
    // used to launch the asynchronous operation.
    auto op = async_write_messages(socket,
                                   "Testing deferred\r\n", 5, boost::asio::deferred);

    // Launch the operation using a lambda as a callback.
    std::move(op)(
            [](const boost::system::error_code& error) {
                if (!error) {
                    std::cout << "Messages sent\n";
                } else {
                    std::cout << "Error: " << error.message() << "\n";
                }
            });

    io_context.run();
}

//------------------------------------------------------------------------------

void test_future() {
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, {tcp::v4(), 55555});
    tcp::socket socket = acceptor.accept();

    // Test our asynchronous operation using the use_future completion token.
    // This token causes the operation's initiating function to return a future,
    // which may be used to synchronously wait for the result of the operation.
    std::future<void> f = async_write_messages(
            socket, "Testing future\r\n", 5, boost::asio::use_future);

    io_context.run();

    try {
        // Get the result of the operation.
        f.get();
        std::cout << "Messages sent\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

//------------------------------------------------------------------------------

int main() {
    test_callback();
    test_deferred();
    test_future();
}
