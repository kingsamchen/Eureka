/*
 @ 0xCCCCCCCC
*/

#include <exception>
#include <functional>
#include <iostream>
#include <memory>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/write.hpp"
#include "gflags/gflags.h"

using asio::ip::tcp;

DEFINE_uint32(port, 9876, "listening port");

std::string MakeDaytime() {
    time_t now = time(nullptr);
    return std::string(ctime(&now));
}

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(asio::io_context& ctx)
        : sock_(ctx) {}

    void Start() {
        message_ = MakeDaytime();
        asio::async_write(
                sock_,
                asio::buffer(message_),
                // Note: see handler.
                std::bind(&Connection::OnWrite, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

    tcp::socket& socket() {
        return sock_;
    }

    void OnWrite(const asio::error_code& /*error*/, size_t /*bytes_transferred*/) {}

private:
    tcp::socket sock_;
    std::string message_;
};

class Server {
public:
    Server(asio::io_context& ctx, uint16_t port)
        : ctx_(ctx),
          acceptor_(ctx, tcp::endpoint(tcp::v4(), port)) {}

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void Start() {
        NewAccept();
    }

private:
    void NewAccept() {
        auto conn = std::make_shared<Connection>(ctx_);
        acceptor_.async_accept(
                conn->socket(),
                std::bind(&Server::HandleNewConnection, this, conn, std::placeholders::_1));
    }

    void HandleNewConnection(const std::shared_ptr<Connection>& conn, const asio::error_code& ec) {
        std::cout << "New connection\n";
        if (!ec) {
            conn->Start();
        }
        NewAccept();
    }

private:
    asio::io_context& ctx_;
    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    try {
        auto port = static_cast<uint16_t>(FLAGS_port);
        asio::io_context ioc;
        Server srv(ioc, port);
        srv.Start();
        ioc.run();
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}