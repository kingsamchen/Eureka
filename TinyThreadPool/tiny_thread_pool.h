/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TINY_THREAD_POOL_H_
#define TINY_THREAD_POOL_H_

#include <condition_variable>
#include <deque>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include "kbase/basic_macros.h"
#include "kbase/error_exception_util.h"

template<typename C>
class ThreadsJoiner {
public:
    explicit ThreadsJoiner(C& threads) noexcept
        : threads_(threads)
    {}

    ~ThreadsJoiner()
    {
        for (auto& th : threads_) {
            th.join();
        }
    }

    DISALLOW_COPY(ThreadsJoiner);

    DEFAULT_MOVE(ThreadsJoiner);

private:
    C& threads_;
};

enum class TaskShutdownBehavior {
    BlockShutdown,
    SkipOnShutdown
};

class TinyThreadPool {
private:
    using Task = std::pair<std::function<void()>, TaskShutdownBehavior>;

public:
    explicit TinyThreadPool(size_t num);

    ~TinyThreadPool();

    DISALLOW_COPY(TinyThreadPool);

    template<typename F, typename... Args>
    std::future<std::result_of_t<F(Args...)>> PostTask(F&& fn, Args&&... args)
    {
        return PostTaskWithShutdownBehavior(TaskShutdownBehavior::BlockShutdown,
                                            std::forward<F>(fn),
                                            std::forward<Args>(args)...);
    }

    template<typename F, typename... Args>
    std::future<std::result_of_t<F(Args...)>> PostTaskWithShutdownBehavior(
        TaskShutdownBehavior behavior, F&& fn, Args&&... args)
    {
        using R = std::result_of_t<F(Args...)>;

        // We have to manage the packaged_task with shared_ptr, because std::function<>
        // requires being copy-constructible and copy-assignable.
        auto task_fn = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<F>(fn), std::forward<Args>(args)...));

        auto future = task_fn->get_future();

        Task task([task_fn=std::move(task_fn)] { (*task_fn)(); }, behavior);

        {
            std::lock_guard<std::mutex> lock(pool_mutex_);
            ENSURE(CHECK, running_).Require();
            task_queue_.push_back(std::move(task));
        }

        not_empty_.notify_one();

        return future;
    }

private:
    void RunInThread();

    Task RetrieveTask();

private:
    std::mutex pool_mutex_;
    std::condition_variable not_empty_;
    std::deque<Task> task_queue_;
    bool running_;
    std::vector<std::thread> threads_;
    ThreadsJoiner<decltype(threads_)> thread_joiner_;
};

#endif  // TINY_THREAD_POOL_H_
