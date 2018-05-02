/*
 @ 0xCCCCCCCC
*/

#include <atomic>
#include <cassert>
#include <thread>
#include <iostream>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed)) {
        ;
    }

    if (x.load(std::memory_order_relaxed)) {
        ++z;
    }
}

bool RunRelaxed()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    return z.load() != 0;
}

void TestRelaxed()
{
    size_t i = 0;
    while (true) {
        ++i;
        if (!RunRelaxed()) {
            break;
        }
    }

    std::cout << "Consistency detected after " << i << " runs!";
}
