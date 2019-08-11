/*
 @ 0xCCCCCCCC
*/

#include <array>
#include <iostream>

#include "kbase/command_line.h"

#include "asio/ip/tcp.hpp"
#include "asio/io_context.hpp"

void RunCli()
{
    auto cmd = kbase::CommandLine::ForCurrentProcess();
    const auto& ip = cmd.GetParameter(0);
    auto port = static_cast<unsigned short>(cmd.GetParameterAs<int>(1));

    asio::io_context ctx;

    asio::ip::tcp::endpoint addr {asio::ip::make_address(ip), port};
    asio::ip::tcp::socket conn_sock(ctx);
    conn_sock.connect(addr);

    while (true) {
        std::array<char, 256> buf;
        asio::error_code ec;
        // short-read may occur for read_some()
        auto len = conn_sock.read_some(asio::buffer(buf), ec);
        if (ec == asio::error::eof) {
            std::cout << "Connection is closed by peer; len=" << len << std::endl;
            return;
        } else if (ec) {
            throw std::system_error(ec);
        }
        std::cout << "[recv]" << std::string(buf.data(), len) << std::endl;
    }
}

int main(int argc, const char* argv[])
{
    kbase::CommandLine::Init(argc, argv);

    try {
        RunCli();
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}