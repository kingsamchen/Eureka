/*
 @ 0xCCCCCCCC
*/

#include <chrono>
#include <iostream>
#include <functional>
#include <thread>

#include "asio/bind_executor.hpp"
#include "asio/io_context.hpp"
#include "asio/strand.hpp"
#include "asio/steady_timer.hpp"

using namespace std::chrono_literals;

class Printer {
public:
    explicit Printer(asio::io_context& ctx)
        : strand_(ctx),
          timer1_(ctx, 2s),
          timer2_(ctx, 2s),
          count_(0)
    {
        timer1_.async_wait(asio::bind_executor(strand_, std::bind(&Printer::Print1, this)));
        timer2_.async_wait(asio::bind_executor(strand_, std::bind(&Printer::Print2, this)));
    }

    ~Printer()
    {
        std::cout << "final count=" << count_ << std::endl;
    }

    Printer(const Printer&) = delete;
    Printer& operator=(const Printer&) = delete;

    void Print1()
    {
        if (count_ < 10) {
            std::cout << "current count=" << count_ << std::endl;
            ++count_;
            timer1_.expires_after(2s);
            timer1_.async_wait(asio::bind_executor(strand_, std::bind(&Printer::Print1, this)));
        }
    }

    void Print2()
    {
        if (count_ < 10) {
            std::cout << "current count=" << count_ << std::endl;
            ++count_;
            timer2_.expires_after(2s);
            timer2_.async_wait(asio::bind_executor(strand_, std::bind(&Printer::Print2, this)));
        }
    }

private:
    asio::io_context::strand strand_;
    asio::steady_timer timer1_;
    asio::steady_timer timer2_;
    int count_;
};

int main()
{
    asio::io_context ioc;

    Printer p(ioc);

    std::thread t([&] {
       ioc.run();
    });

    ioc.run();

    t.join();

    return 0;
}
