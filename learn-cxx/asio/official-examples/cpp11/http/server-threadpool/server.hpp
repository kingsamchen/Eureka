//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_SERVER_HPP
#define HTTP_SERVER3_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include "request_handler.hpp"

namespace http {
namespace server3 {

/// The top-level class of the HTTP server.
class server {
public:
    server(const server&) = delete;
    server& operator=(const server&) = delete;

    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit server(const std::string& address, const std::string& port,
                    const std::string& doc_root, std::size_t thread_pool_size);

    /// Run the server's io_context loop.
    void run();

private:
    /// Perform an asynchronous accept operation.
    void do_accept();

    /// Wait for a request to stop the server.
    void do_await_stop();

    /// The number of threads that will call io_context::run().
    std::size_t thread_pool_size_;

    /// The io_context used to perform asynchronous operations.
    boost::asio::io_context io_context_;

    /// The signal_set is used to register for process termination notifications.
    boost::asio::signal_set signals_;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    /// The handler for all incoming requests.
    request_handler request_handler_;
};

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_SERVER_HPP
