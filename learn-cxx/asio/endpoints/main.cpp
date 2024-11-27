/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "asio.hpp"

void sample1() {
    [[maybe_unused]] auto any = asio::ip::address_v4::any();
    [[maybe_unused]] auto bc_addr = asio::ip::address_v4::broadcast();
    [[maybe_unused]] auto loopback = asio::ip::address_v6::loopback();

    asio::ip::tcp::endpoint ep;
}

void test_poll() {
    asio::io_context ctx;

    asio::signal_set sig(ctx, SIGINT, SIGTERM);
    sig.async_wait([](auto, auto) {
        std::cout << "signaled\n";
    });

    ctx.poll();
}

int main() {
    return 0;
}