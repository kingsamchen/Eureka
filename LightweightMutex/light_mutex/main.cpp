/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <mutex>
#include <thread>

#include "light_mutex/light_mutex.h"

void TestMutualExclusive()
{
    int value = 0;
    LightMutex light_mutex;

    auto fn = [&] {
        for (int i = 1; i <= 100; ++i) {
            {
                std::lock_guard<LightMutex> lock(light_mutex);
                value += i;
            }
        }
    };

    std::thread th1(fn);
    std::thread th2(fn);

    th1.join();
    th2.join();

    std::cout << "value=" << value;
}

void TestTryLock()
{
    LightMutex mtx;
    mtx.lock();
    bool r = mtx.try_lock() == false;
    assert(r);
    mtx.unlock();
    r = mtx.try_lock() == true;
    assert(r);
}

int main()
{
    TestMutualExclusive();
    TestTryLock();

    return 0;
}