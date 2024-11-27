/*
 @ 0xCCCCCCCC
*/

#include <exception>
#include <iostream>

#include "asio/buffer.hpp"
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

void RunServer() {
    asio::io_context ctx;
    tcp::acceptor acceptor(ctx, tcp::endpoint{tcp::v4(), static_cast<uint16_t>(FLAGS_port)});

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

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    try {
        RunServer();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}