/*
 @ 0xCCCCCCCC
*/

#include <cstdlib>
#include <future>
#include <iostream>

#include "blocking_queue.h"

const int kSleepTime = 1000;

template<typename T>
void Produce(BlockingQueue<T>& queue)
{
    while (true) {
        auto item = rand() % 1000;
        std::cout << "produced item " << item << std::endl;
        queue.Push(item);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500));
    }
}

template<typename T>
void Consume(BlockingQueue<T>& queue)
{
    while (true) {
        auto item = queue.Pop();
        std::cout << "consume item " << item << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
    }
}


int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    BlockingQueue<int> bq;

    auto f_p(std::async(std::launch::async, Produce<int>, std::ref(bq)));
    auto f_c(std::async(std::launch::async, Consume<int>, std::ref(bq)));
    auto f_c2(std::async(std::launch::async, Consume<int>, std::ref(bq)));

    return 0;
}