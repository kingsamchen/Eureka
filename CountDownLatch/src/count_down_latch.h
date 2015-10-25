/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EUREKA_COUNT_DOWN_LATCH_H_
#define EUREKA_COUNT_DOWN_LATCH_H_

#include <condition_variable>
#include <mutex>

#include "compiler_helper.h"

class CountDownLatch {
public:
    explicit CountDownLatch(int count);

    ~CountDownLatch() = default;

    DISALLOW_COPY(CountDownLatch);

    DISALLOW_MOVE(CountDownLatch);

    void Wait();

    void Countdown();

    int count() const;

private:
    int count_;
    mutable std::mutex mutex_;
    std::condition_variable latch_;
};

#endif
