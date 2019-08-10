/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <chrono>

#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"

int main()
{
    using namespace std::chrono_literals;
    asio::io_context ioc;
    asio::steady_timer timer(ioc, 3s);

    timer.async_wait([](const auto&) {
        std::cout << "async wait completed" << std::endl;
    });

    ioc.run();

    return 0;
}