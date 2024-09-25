// Kingsley Chen

#pragma once

#include <condition_variable>
#include <coroutine>
#include <cstddef>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

#include "fmt/format.h"
#include "fmt/std.h"

namespace coro {

class thread_pool {
public:
    explicit thread_pool(std::size_t worker_cnt) {
        thds_.resize(worker_cnt);
        for (std::size_t i = 0; i < worker_cnt; ++i) {
            std::jthread thd([this] {
                worker_loop();
            });
            thds_.push_back(std::move(thd));
        }
    }

    ~thread_pool() {
        shutdown();
    }

    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool& operator=(thread_pool&&) = delete;

    auto schedule() {
        struct awaiter {
            thread_pool* tp;

            [[nodiscard]] constexpr bool await_ready() const noexcept {
                (void)this;
                return false;
            }

            constexpr void await_resume() const noexcept {}

            void await_suspend(std::coroutine_handle<> coro) const {
                tp->enqueue_task(coro);
            }
        };

        return awaiter{this};
    }

private:
    void worker_loop() {
        fmt::println("worker-{} starts running", std::this_thread::get_id());

        while (true) {
            std::unique_lock<std::mutex> lock(mtx_);
            not_empty_.wait(lock, [this] { return stopped_ || !coros_.empty(); });
            if (stopped_) {
                break;
            }

            auto coro = coros_.front();
            coros_.pop_front();

            lock.unlock();
            coro.resume();
        }

        fmt::println("worker-{} stopped", std::this_thread::get_id());
    }

    void enqueue_task(std::coroutine_handle<> coro) {
        {
            std::scoped_lock<std::mutex> lock(mtx_);
            coros_.push_back(coro);
        }
        not_empty_.notify_one();
    }

    void shutdown() {
        {
            std::scoped_lock<std::mutex> lock(mtx_);
            stopped_ = true;
        }
        not_empty_.notify_all();
    }

    std::mutex mtx_;
    std::condition_variable not_empty_;
    bool stopped_{false};                       // Guarded by `mtx_`
    std::deque<std::coroutine_handle<>> coros_; // Guarded by `mtx_`
    std::vector<std::jthread> thds_;
};

} // namespace coro
