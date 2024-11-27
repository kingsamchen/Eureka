/*
 @ 0xCCCCCCCC
*/

#include <array>
#include <iostream>

#include "argparse/argparse.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"

void RunCli(const argparse::ArgumentParser& args) {
    asio::io_context ctx;

    asio::ip::tcp::endpoint addr{asio::ip::make_address(args.get("ip")),
                                 args.get<uint16_t>("port")};
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

int main(int argc, const char* argv[]) {
    argparse::ArgumentParser program("daytime-cli");
    program.add_argument("ip")
            .required()
            .help("remote ip addr");
    program.add_argument("port")
            .required()
            .action([](const std::string& value) { return static_cast<uint16_t>(std::stoi(value)); })
            .help("remote port");

    try {
        program.parse_args(argc, argv);
        RunCli(program);
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
