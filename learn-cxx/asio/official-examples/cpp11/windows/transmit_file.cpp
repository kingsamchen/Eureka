//
// transmit_file.cpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#if defined(BOOST_ASIO_HAS_WINDOWS_OVERLAPPED_PTR)

using boost::asio::ip::tcp;
using boost::asio::windows::overlapped_ptr;
using boost::asio::windows::random_access_handle;

typedef boost::asio::basic_stream_socket<tcp,
                                         boost::asio::io_context::executor_type>
        tcp_socket;

typedef boost::asio::basic_socket_acceptor<tcp,
                                           boost::asio::io_context::executor_type>
        tcp_acceptor;

// A wrapper for the TransmitFile overlapped I/O operation.
template<typename Handler>
void transmit_file(tcp_socket& socket,
                   random_access_handle& file, Handler handler) {
    // Construct an OVERLAPPED-derived object to contain the handler.
    overlapped_ptr overlapped(socket.get_executor().context(), handler);

    // Initiate the TransmitFile operation.
    BOOL ok = ::TransmitFile(socket.native_handle(),
                             file.native_handle(), 0, 0, overlapped.get(), 0, 0);
    DWORD last_error = ::GetLastError();

    // Check if the operation completed immediately.
    if (!ok && last_error != ERROR_IO_PENDING) {
        // The operation completed immediately, so a completion notification needs
        // to be posted. When complete() is called, ownership of the OVERLAPPED-
        // derived object passes to the io_context.
        boost::system::error_code ec(last_error,
                                     boost::asio::error::get_system_category());
        overlapped.complete(ec, 0);
    } else {
        // The operation was successfully initiated, so ownership of the
        // OVERLAPPED-derived object has passed to the io_context.
        overlapped.release();
    }
}

class connection
    : public std::enable_shared_from_this<connection> {
public:
    typedef std::shared_ptr<connection> pointer;

    static pointer create(boost::asio::io_context& io_context,
                          const std::string& filename) {
        return pointer(new connection(io_context, filename));
    }

    tcp_socket& socket() {
        return socket_;
    }

    void start() {
        boost::system::error_code ec;
        file_.assign(::CreateFile(filename_.c_str(), GENERIC_READ, 0, 0,
                                  OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0),
                     ec);
        if (file_.is_open()) {
            transmit_file(socket_, file_,
                          std::bind(&connection::handle_write, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
        }
    }

private:
    connection(boost::asio::io_context& io_context, const std::string& filename)
        : socket_(io_context),
          filename_(filename),
          file_(io_context) {
    }

    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/) {
        boost::system::error_code ignored_ec;
        socket_.shutdown(tcp_socket::shutdown_both, ignored_ec);
    }

    tcp_socket socket_;
    std::string filename_;
    random_access_handle file_;
};

class server {
public:
    server(boost::asio::io_context& io_context,
           unsigned short port, const std::string& filename)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          filename_(filename) {
        start_accept();
    }

private:
    void start_accept() {
        connection::pointer new_connection =
                connection::create(acceptor_.get_executor().context(), filename_);

        acceptor_.async_accept(new_connection->socket(),
                               std::bind(&server::handle_accept, this, new_connection,
                                         boost::asio::placeholders::error));
    }

    void handle_accept(connection::pointer new_connection,
                       const boost::system::error_code& error) {
        if (!error) {
            new_connection->start();
        }

        start_accept();
    }

    tcp_acceptor acceptor_;
    std::string filename_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: transmit_file <port> <filename>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        using namespace std; // For atoi.
        server s(io_context, atoi(argv[1]), argv[2]);

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

#else // defined(BOOST_ASIO_HAS_WINDOWS_OVERLAPPED_PTR)
#error Overlapped I/O not available on this platform
#endif // defined(BOOST_ASIO_HAS_WINDOWS_OVERLAPPED_PTR)
