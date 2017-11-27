/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <thread>

#include "kbase/scope_guard.h"

void RaiseException()
{
    throw std::runtime_error("god");
}

void LaunchAndRaise()
{
    using namespace std::chrono_literals;

    std::thread th([] {
        std::cout << "worker thread is working\n";
        auto start = std::chrono::system_clock::now();
        while (std::chrono::system_clock::now() - start < 1s) {}
        std::cout << "work is done" << std::endl;
    });

    ON_SCOPE_EXIT{
        if (th.joinable()) {
            th.join();
        }
    };

    std::cout << "Let's raise an exception!\n";

    RaiseException();

    std::cout << "Normal return from LaunchAndRaise()!\n";
}

void SafeWaitThreadComplete()
{
    try {
        LaunchAndRaise();
    } catch (const std::exception& ex) {
        std::cout << "Handle exception: " << ex.what() << std::endl;
    }

    std::cout << "SafeWaitThreadComplete() returns!\n";
}
