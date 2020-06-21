
#include <cstdlib>
#include <iostream>
#include <string>

#include "argparse.hpp"

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/signal_set.hpp"

#include "kbase/logging.h"

int main(int argc, const char* argv[])
{
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

    kbase::LoggingSettings settings;
    settings.logging_destination = kbase::LogToSystemDebugLog;
    kbase::ConfigureLoggingSettings(settings);

    try {
        asio::io_context io_ctx;

        asio::signal_set signals(io_ctx, SIGINT, SIGTERM);
        signals.async_wait([&io_ctx](std::error_code, int) {
            io_ctx.stop();
            LOG(INFO) << "Bye-bye";
        });

        asio::ip::tcp::endpoint addr(asio::ip::tcp::v4(), port);
        LOG(INFO) << "Listening at " << addr;

        // TODO: Make proxy instance and run

        io_ctx.run();
    } catch (const std::exception& ex) {
        LOG(ERROR) << "Critical failure: " << ex.what();
        return EXIT_FAILURE;
    }

    return 0;
}
