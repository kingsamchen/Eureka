/*
 @ 0xCCCCCCCC
*/

#include "hierarchical_mutex.h"

#include <iostream>

HierarchicalMutex g_low_mutex(100);
HierarchicalMutex g_high_mutex(200);

void FnA()
{
    std::lock_guard<HierarchicalMutex> hlock(g_high_mutex);
    std::cout << "Thread A acquire high-mutex" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::lock_guard<HierarchicalMutex> llock(g_low_mutex);
    std::cout << "Thread A acquire low-mutex" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Thread A exit" << std::endl;
}

void FnB()
{
    std::lock_guard<HierarchicalMutex> lock(g_high_mutex);
    std::cout << "Thread B acquire high-mutex" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Thread B exit" << std::endl;
}

void TestHierarchicalMutex()
{
    std::thread t1(FnA);
    std::thread t2(FnB);

    std::cout << "Join Threads" << std::endl;

    t1.join();
    t2.join();
}
