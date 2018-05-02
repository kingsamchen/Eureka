/*
 @ 0xCCCCCCCC
*/

#include "blocking_queue_with_nt_cond.h"

#include <iostream>
#include <random>
#include <thread>

// We skip any possible wrappers and manipulate on raw Windows API level.

MTQueue::MTQueue(size_t capacity)
    : capacity_(capacity)
{
    InitializeCriticalSection(&queue_mutex_);
    InitializeConditionVariable(&not_empty_);
    InitializeConditionVariable(&not_full_);
}

MTQueue::~MTQueue()
{
    DeleteCriticalSection(&queue_mutex_);
}

void MTQueue::Put(int data)
{
    auto tid = std::this_thread::get_id();

    EnterCriticalSection(&queue_mutex_);

    while (full()) {
        std::cout << "Queue is full; block thread " << tid << std::endl;
        SleepConditionVariableCS(&not_full_, &queue_mutex_, INFINITE);
    }

    std::cout << "Thread " << tid << " enqueue " << data << std::endl;
    queue_.push(data);

    LeaveCriticalSection(&queue_mutex_);

    WakeConditionVariable(&not_empty_);
}

int MTQueue::Pop()
{
    auto tid = std::this_thread::get_id();

    EnterCriticalSection(&queue_mutex_);

    while (empty()) {
        std::cout << "Queue is empty; block thread " << tid << std::endl;
        SleepConditionVariableCS(&not_empty_, &queue_mutex_, INFINITE);
    }

    int data = queue_.back();
    queue_.pop();
    std::cout << "Thread " << tid << " retrieve data " << data << std::endl;

    LeaveCriticalSection(&queue_mutex_);

    WakeConditionVariable(&not_full_);

    return data;
}

// Both not thread-safe

bool MTQueue::empty() const
{
    return queue_.empty();
}

bool MTQueue::full() const
{
    return queue_.size() >= capacity_;
}

// -*- test stub code -*-

constexpr size_t kCap = 10;

void Produce(MTQueue& mtq)
{
    std::random_device rd;
    std::default_random_engine engine (rd());
    std::uniform_int_distribution<> dist(1, 1000);

    for (size_t i = 0; i < kCap * 2; ++i) {
        mtq.Put(dist(engine));
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    for (size_t i = 0; i < kCap * 2; ++i) {
        mtq.Put(dist(engine));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Signal to exit.
    mtq.Put(-1);
    mtq.Put(-1);
}

void Consumer(MTQueue& mtq)
{
    // Stage 1
    for (size_t i = 0; i < 8; ++i) {
        mtq.Pop();
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    // Stage 2
    while (true) {
        auto data = mtq.Pop();
        if (data == -1) {
            std::cout << "Consumer exit!" << std::endl;
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void TestMTQueue()
{
    MTQueue mtq(kCap);

    std::thread producer(Produce, std::ref(mtq));

    std::thread consumers[2] {std::thread(Consumer, std::ref(mtq)),
                              std::thread(Consumer, std::ref(mtq))};

    producer.join();
    consumers[0].join();
    consumers[1].join();

    std::cout << "All complete!" << std::endl;
}