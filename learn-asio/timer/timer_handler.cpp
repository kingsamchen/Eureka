/*
 @ 0xCCCCCCCC
*/

#include <chrono>
#include <functional>
#include <iostream>

#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"

using namespace std::chrono_literals;

void TimerTick(asio::steady_timer& timer, int& count, const asio::error_code&)
{
    std::cout << "current count=" << count << std::endl;
    ++count;
    if (count < 5) {
        timer.expires_after(1s);
        timer.async_wait(
            std::bind(&TimerTick, std::ref(timer), std::ref(count), std::placeholders::_1));
    }
}

int main()
{
    asio::io_context ioc;
    asio::steady_timer timer(ioc, 1s);

    int run_count = 0;

    timer.async_wait(
        std::bind(&TimerTick, std::ref(timer), std::ref(run_count), std::placeholders::_1));

    ioc.run();

    std::cout << "final count=" << run_count << std::endl;

    return 0;
}