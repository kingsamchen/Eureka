/*
 @ 0xCCCCCCCC
*/

#include <chrono>
#include <iostream>

#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"

int main()
{
    asio::io_context ioc;
    asio::steady_timer timer(ioc, std::chrono::seconds(3));

    timer.wait();

    std::cout << "sync wait completed" << std::endl;

    return 0;
}