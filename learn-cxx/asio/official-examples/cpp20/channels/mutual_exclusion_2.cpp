//
// mutual_exclusion_2.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <boost/asio/experimental/channel.hpp>
#include <iostream>
#include <memory>

using boost::asio::as_tuple;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::dynamic_buffer;
using boost::asio::io_context;
using boost::asio::steady_timer;
using boost::asio::experimental::channel;
using boost::asio::ip::tcp;
using namespace boost::asio::buffer_literals;
using namespace std::literals::chrono_literals;

// This class implements a simple line-based protocol:
//
// * For event line that is received from the client, the session sends a
//   message header followed by the content of the line as the message body.
//
// * The session generates heartbeat messages once a second.
//
// This protocol is implemented using two actors, handle_messages() and
// send_heartbeats(), each written as a coroutine.
class line_based_echo_session : public std::enable_shared_from_this<line_based_echo_session> {
    // The socket used to read from and write to the client. This socket is a
    // data member as it is shared between the two actors.
    tcp::socket socket_;

    // As both of the actors will write to the socket, we need a lock to prevent
    // these writes from overlapping. To achieve this, we use a channel with a
    // buffer size of one. The lock is claimed by sending a message to the
    // channel, and then released by receiving this message back again. If the
    // lock is not held then the channel's buffer is empty, and the send will
    // complete without delay. Otherwise, if the lock is held by the other actor,
    // then the send operation will not complete until the lock is released.
    channel<void()> write_lock_{socket_.get_executor(), 1};

public:
    line_based_echo_session(tcp::socket socket)
        : socket_{std::move(socket)} {
        socket_.set_option(tcp::no_delay(true));
    }

    void start() {
        co_spawn(socket_.get_executor(), [self = shared_from_this()] { return self->handle_messages(); }, detached);

        co_spawn(socket_.get_executor(), [self = shared_from_this()] { return self->send_heartbeats(); }, detached);
    }

private:
    void stop() {
        socket_.close();
        write_lock_.cancel();
    }

    awaitable<void> handle_messages() {
        try {
            constexpr std::size_t max_line_length = 1024;
            std::string data;
            for (;;) {
                // Read an entire line from the client.
                std::size_t length = co_await async_read_until(socket_,
                                                               dynamic_buffer(data, max_line_length), '\n');

                // Claim the write lock by sending a message to the channel. Since the
                // channel signature is void(), there are no arguments to send in the
                // message itself. In this example we optimise for the common case,
                // where the lock is not held by the other actor, by first trying a
                // non-blocking send.
                if (!write_lock_.try_send()) {
                    co_await write_lock_.async_send();
                }

                // Respond to the client with a message, echoing the line they sent.
                co_await async_write(socket_, "<line>"_buf);
                co_await async_write(socket_, dynamic_buffer(data, length));

                // Release the lock by receiving the message back again.
                write_lock_.try_receive([](auto...) {});
            }
        } catch (const std::exception&) {
            stop();
        }
    }

    awaitable<void> send_heartbeats() {
        steady_timer timer{socket_.get_executor()};
        try {
            for (;;) {
                // Wait one second before trying to send the next heartbeat.
                timer.expires_after(1s);
                co_await timer.async_wait();

                // Claim the write lock by sending a message to the channel. Since the
                // channel signature is void(), there are no arguments to send in the
                // message itself. In this example we optimise for the common case,
                // where the lock is not held by the other actor, by first trying a
                // non-blocking send.
                if (!write_lock_.try_send()) {
                    co_await write_lock_.async_send();
                }

                // Send a heartbeat to the client. As the content of the heartbeat
                // message never varies, a buffer literal can be used to specify the
                // bytes of the message. The memory associated with a buffer literal is
                // valid for the lifetime of the program, which mean that the buffer
                // can be safely passed as-is to the asynchronous operation.
                co_await async_write(socket_, "<heartbeat>\n"_buf);

                // Release the lock by receiving the message back again.
                write_lock_.try_receive([](auto...) {});
            }
        } catch (const std::exception&) {
            stop();
        }
    }
};

awaitable<void> listen(tcp::acceptor& acceptor) {
    for (;;) {
        auto [e, socket] = co_await acceptor.async_accept(as_tuple);
        if (!e) {
            std::make_shared<line_based_echo_session>(std::move(socket))->start();
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: mutual_exclusion_1";
            std::cerr << " <listen_address> <listen_port>\n";
            return 1;
        }

        io_context ctx;

        auto listen_endpoint =
                *tcp::resolver(ctx).resolve(argv[1], argv[2],
                                            tcp::resolver::passive)
                         .begin();

        tcp::acceptor acceptor(ctx, listen_endpoint);
        co_spawn(ctx, listen(acceptor), detached);
        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}
