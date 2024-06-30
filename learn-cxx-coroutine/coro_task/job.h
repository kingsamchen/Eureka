//
// 0xCCCCCCCC
//

#pragma once

#include <coroutine>
#include <cstdlib>
#include <utility>

#include "fmt/format.h"

namespace coro {

class job {
    struct promise {
        job get_return_object() {
            return job(std::coroutine_handle<promise>::from_promise(*this));
        }

        std::suspend_always initial_suspend() noexcept {
            (void)this;
            fmt::println("Prepare the job");
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            (void)this;
            fmt::println("Execute the job");
            return {};
        }

        void return_void() noexcept {}

        void unhandled_exception() {
            (void)this;
            std::abort();
        }
    };

public:
    using promise_type = promise;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit job(handle_type handle) noexcept
        : handle_(handle) {}

    job(job&& other) noexcept
        : handle_(std::exchange(other.handle_, nullptr)) {}

    job& operator=(job&& rhs) noexcept {
        job tmp(std::move(rhs));
        std::swap(handle_, tmp.handle_);
        return *this;
    }

    ~job() {
        if (handle_) {
            handle_.destroy();
        }
    }

    job(const job&) = delete;
    job& operator=(const job&) = delete;

    void start() {
        handle_.resume();
    }

private:
    handle_type handle_;
};

} // namespace coro
