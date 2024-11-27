
#include <cstdlib>
#include <iostream>
#include <string>

#include "argparse/argparse.hpp"

#include "asio/io_context.hpp"
#include "asio/signal_set.hpp"

#include "glog/logging.h"

#include "proxy.h"

int main(int argc, const char* argv[]) {
    google::InitGoogleLogging(argv[0]);

    argparse::ArgumentParser program("socks5-proxy");
    program.add_argument("-p", "--port")
            .required()
            .action([](const std::string& value) { return static_cast<uint16_t>(std::stoi(value)); })
            .help("listening port");

    uint16_t port;
    try {
        program.parse_args(argc, argv);
        port = program.get<uint16_t>("--port");
    } catch (const std::exception& ex) {
        std::cout << "Parsing arguments failure: " << ex.what() << std::endl;
        std::cout << program;
        return EXIT_FAILURE;
    }

    try {
        asio::io_context io_ctx;

        Proxy proxy(io_ctx, port);
        proxy.Start();

        asio::signal_set signals(io_ctx, SIGINT, SIGTERM);
        signals.async_wait([&proxy](std::error_code, int) {
            proxy.Stop();
        });

        io_ctx.run();

        LOG(INFO) << "Bye-bye";
    } catch (const std::exception& ex) {
        LOG(ERROR) << "Critical failure: " << ex.what();
        return EXIT_FAILURE;
    }

    return 0;
}
