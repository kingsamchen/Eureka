/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "asio/io_context.hpp"
#include "asio/signal_set.hpp"

#include "kbase/command_line.h"
#include "kbase/logging.h"

#include "proxy.h"

int main(int argc, const char* argv[])
{
    kbase::CommandLine::Init(argc, argv);

    kbase::LoggingSettings settings;
    settings.logging_destination = kbase::LogToSystemDebugLog;
    kbase::ConfigureLoggingSettings(settings);

    asio::io_context ioc;

    asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](std::error_code, int) {
        ioc.stop();
        std::cout << "Bye-bye" << std::endl;
    });

    asio::ip::tcp::endpoint addr(asio::ip::tcp::v4(), 9876);
    Proxy proxy(ioc, addr);
    proxy.Start();

    ioc.run();

    return 0;
}
