/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "asio/io_context.hpp"
#include "asio/signal_set.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "proxy.h"

DEFINE_uint32(port, 9876, "listening port");

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    asio::io_context ioc;

    asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](std::error_code, int) {
        ioc.stop();
        std::cout << "Bye-bye" << std::endl;
    });

    auto port = static_cast<std::uint16_t>(FLAGS_port);
    asio::ip::tcp::endpoint addr(asio::ip::tcp::v4(), port);
    Proxy proxy(ioc, addr);
    proxy.Start();

    ioc.run();

    return 0;
}
