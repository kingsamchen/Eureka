/*
 @ 0xCCCCCCCC
*/

#include "count_down_latch.h"

#include <cassert>

CountDownLatch::CountDownLatch(int count)
{
    assert(count >= 0);
    count_ = count;
}

void CountDownLatch::Wait()
{
    std::unique_lock<std::mutex> lock(mutex_);
    latch_.wait(lock, [this] { return count_ <= 0; });
}

void CountDownLatch::Countdown()
{
    std::lock_guard<std::mutex> lock(mutex_);
    --count_;
    if (count_ == 0) {
        latch_.notify_all();
    }
}

int CountDownLatch::count() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return count_;
}