/*
 @ 0xCCCCCCCC
*/

#include <exception>
#include <iostream>
#include <memory>
#include <functional>

#include "kbase/basic_macros.h"
#include "kbase/command_line.h"

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/write.hpp"

using asio::ip::tcp;

std::string MakeDaytime()
{
    time_t now = time(nullptr);
    return std::string(ctime(&now));
}

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(asio::io_context& ctx)
        : sock_(ctx)
    {}

    void Start()
    {
        message_ = MakeDaytime();
        asio::async_write(
            sock_,
            asio::buffer(message_),
            // Note: see handler.
            std::bind(&Connection::OnWrite, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

    tcp::socket& socket()
    {
        return sock_;
    }

    void OnWrite(const asio::error_code& /*error*/, size_t /*bytes_transferred*/)
    {}

private:
    tcp::socket sock_;
    std::string message_;
};

class Server {
public:
    Server(asio::io_context& ctx, uint16_t port)
        : acceptor_(ctx, tcp::endpoint(tcp::v4(), port))
    {}

    DISALLOW_COPY(Server);

    void Start()
    {
        NewAccept();
    }

private:
    void NewAccept()
    {
        auto conn = std::make_shared<Connection>(acceptor_.get_executor().context());
        acceptor_.async_accept(
            conn->socket(),
            std::bind(&Server::HandleNewConnection, this, conn, std::placeholders::_1));
    }

    void HandleNewConnection(const std::shared_ptr<Connection>& conn, const asio::error_code& ec)
    {
        std::cout << "New connection\n";
        if (!ec) {
            conn->Start();
        }
        NewAccept();
    }

private:
    tcp::acceptor acceptor_;
};

int main(int argc, const char* argv[])
{
    kbase::CommandLine::Init(argc, argv);

    try {
        auto port = static_cast<uint16_t>(
            kbase::CommandLine::ForCurrentProcess().GetSwitchValueAs<int>("port", 9876));
        asio::io_context ioc;
        Server srv(ioc, port);
        srv.Start();
        ioc.run();
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}