/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <exception>

#include "asio/buffer.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/write.hpp"

#include "kbase/command_line.h"

using asio::ip::tcp;

std::string MakeDaytime()
{
    time_t now = time(nullptr);
    return std::string(ctime(&now));
}

void RunServer()
{
    asio::io_context ctx;

    auto port =
        kbase::CommandLine::ForCurrentProcess().GetSwitchValueAs<int>("port", 9876);
    tcp::acceptor acceptor(ctx, tcp::endpoint{tcp::v4(), static_cast<uint16_t>(port)});

    while (true) {
        tcp::socket conn_sock(ctx);
        acceptor.accept(conn_sock);

        std::string message = MakeDaytime();

        asio::error_code ignored;
        // Notes:
        // 1) why use asio::write() ?
        // 2) what is asio::buffer for ?
        asio::write(conn_sock, asio::buffer(message), ignored);
    }
}

int main(int argc, const char* argv[])
{
    kbase::CommandLine::Init(argc, argv);

    try {
        RunServer();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}