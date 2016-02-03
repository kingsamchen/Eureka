/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef BLOCKING_QUEUE_H_
#define BLOCKING_QUEUE_H_

#include <mutex>
#include <queue>

#include "basic_macros.h"

template<typename T>
class BlockingQueue {
public:
    BlockingQueue() = default;

    ~BlockingQueue() = default;

    DISALLOW_COPY(BlockingQueue);

    DISALLOW_MOVE(BlockingQueue);

    void Push(const T& ele)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer_.push(ele);
        }

        not_empty_.notify_one();
    }

    T Pop()
    {
        static_assert(std::is_nothrow_move_constructible<T>::value &&
                      std::is_nothrow_move_assignable<T>::value,
                      "Element requires no-throw move");
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !buffer_.empty(); });

        auto ele = buffer_.front();
        buffer_.pop();

        return ele;
    }

    void Pop(T* ele)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !buffer_.empty(); });

        *ele = buffer_.front();
        buffer_.pop();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return buffer_.size();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::queue<T> buffer_;
};

#endif  // BLOCKING_QUEUE_H_
