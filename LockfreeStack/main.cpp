
#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include <random>

#include "lstack.h"

void SingleThread()
{
    LStack<int> ls(5);

    ls.Push(1);
    ls.Push(3);
    ls.Push(5);
    ls.Pop();
    ls.Pop();
    ls.Pop();

    assert(!ls.Pop().first);
}

void Multithread()
{
    LStack<int> ls(100);

    std::thread producer([&ls] {
        std::random_device rd;
        std::default_random_engine e(rd());
        std::uniform_int_distribution<int> dist(0, 1000);

        for (int i = 0; i < 50; ++i) {
            auto t = dist(e);
            auto ret = ls.Push(t);
            if (ret) {
                printf("P -> %d\n", t);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(t));
        }
    });

    std::thread consumer([&ls] {
        std::random_device rd;
        std::default_random_engine e(rd());
        std::uniform_int_distribution<int> dist(0, 1000);

        for (int i = 0; i < 50; ++i) {
            auto t = dist(e);
            auto r = ls.Pop();
            if (r.first) {
                printf("C <- %d\n", r.second);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(t));
        }
    });

    producer.join();
    consumer.join();
}

int main()
{
    // SingleThread();
    Multithread();
    return 0;
}

