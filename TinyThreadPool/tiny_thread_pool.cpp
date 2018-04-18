/*
 @ 0xCCCCCCCC
*/

#include "tiny_thread_pool.h"

namespace {

}   // namespace

TinyThreadPool::TinyThreadPool(size_t num)
    : running_(true), thread_joiner_(threads_)
{
    threads_.reserve(num);
    for (size_t i = 0; i < num; ++i) {
        threads_.emplace_back(std::bind(&TinyThreadPool::RunInThread, this));
    }
}

TinyThreadPool::~TinyThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        running_ = false;
    }

    not_empty_.notify_all();
}

void TinyThreadPool::RunInThread()
{
    while (true) {
        Task task(RetrieveTask());

        // The pool is going to shutdown.
        if (!task.first) {
            return;
        }

        task.first();
    }
}

TinyThreadPool::Task TinyThreadPool::RetrieveTask()
{
    Task task;

    std::unique_lock<std::mutex> lock(pool_mutex_);
    not_empty_.wait(lock, [this] { return !running_ || !task_queue_.empty(); });

    while (!task_queue_.empty()) {
        if (!running_ && task_queue_.front().second == TaskShutdownBehavior::SkipOnShutdown) {
            task_queue_.pop_front();
            continue;
        }

        task = std::move(task_queue_.front());
        task_queue_.pop_front();
        break;
    }

    return task;
}
