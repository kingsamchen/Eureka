//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

using boost::asio::ip::tcp;

// Define a helper macro to invoke BOOST_ASIO_HANDLER_LOCATION with the current
// file name, line number, and function name. For the function name, you might
// also consider using __PRETTY_FUNCTION__, BOOST_CURRENT_FUNCTION, or a hand-
// crafted name. For C++20 or later, you may also use std::source_location.
#define HANDLER_LOCATION \
    BOOST_ASIO_HANDLER_LOCATION((__FILE__, __LINE__, __func__))

class session
    : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket socket)
        : socket_(std::move(socket)) {
    }

    void start() {
        HANDLER_LOCATION;

        do_read();
    }

private:
    void do_read() {
        HANDLER_LOCATION;

        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                [this, self](boost::system::error_code ec, std::size_t length) {
                                    HANDLER_LOCATION;

                                    if (!ec) {
                                        do_write(length);
                                    }
                                });
    }

    void do_write(std::size_t length) {
        HANDLER_LOCATION;

        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                     HANDLER_LOCATION;

                                     if (!ec) {
                                         do_read();
                                     }
                                 });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class server {
public:
    server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        HANDLER_LOCATION;

        acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    HANDLER_LOCATION;

                    if (!ec) {
                        std::make_shared<session>(std::move(socket))->start();
                    }

                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        server s(io_context, std::atoi(argv[1]));

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
