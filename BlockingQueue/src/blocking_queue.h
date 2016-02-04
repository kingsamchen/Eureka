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

    BlockingQueue(const BlockingQueue& other)
    {
        std::lock_guard<std::mutex> lock(other.mutex_);
        queue_ = other.queue_;
    }

    BlockingQueue& operator=(const BlockingQueue&) = delete;

    ~BlockingQueue() = default;

    DISALLOW_MOVE(BlockingQueue);

    void Push(const T& ele)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(ele);
        }

        not_empty_.notify_one();
    }

    void Push(T&& ele)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(ele));
        }

        not_empty_.notify_one();
    }

    T Pop()
    {
        static_assert(std::is_nothrow_move_constructible<T>::value &&
                      std::is_nothrow_move_assignable<T>::value,
                      "Element requires no-throw move");
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !queue_.empty(); });

        auto ele = queue_.front();
        queue_.pop();

        return ele;
    }

    void Pop(T* ele)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !queue_.empty(); });

        *ele = queue_.front();
        queue_.pop();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::queue<T> queue_;
};

#endif  // BLOCKING_QUEUE_H_
