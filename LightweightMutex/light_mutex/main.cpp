/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <mutex>
#include <thread>

#include "light_mutex/light_mutex.h"
#include "light_mutex/recursive_light_mutex.h"

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

void BoomUnlockOnNonowningThread()
{
    LightMutex mtx;
    mtx.lock();
    std::thread th([&] {
        mtx.unlock();
    });
    th.join();
}

void TestRecursive()
{
    RecursiveLightMutex mtx;
    mtx.lock();
    std::cout << "main gained the lock\n";
    std::thread th([&] {
        mtx.lock();
        std::cout << "worker gained the lock\n";
        mtx.unlock();
        std::cout << "worker release the lock\n";
    });
    auto r = mtx.try_lock();
    std::cout << "main gained the lock again\n";
    assert(r);
    mtx.unlock();
    std::cout << "main lose the lock\n";
    mtx.unlock();
    std::cout << "main lose the lock again\n";

    th.join();
}

int main()
{
    // TestMutualExclusive();
    //TestTryLock();
    //BoomUnlockOnNonowningThread();
    TestRecursive();
    return 0;
}