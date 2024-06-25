//
// 0xCCCCCCCC
//

#pragma once

#include <coroutine>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <random>
#include <thread>
#include <utility>

#include "fmt/format.h"
#include "fmt/std.h"

namespace coro {

auto rnd_bool = std::bind_front(std::uniform_int_distribution<>(0, 1),
                                std::default_random_engine{std::random_device{}()});

struct my_awaitable {
    std::reference_wrapper<std::jthread> thd;

    bool await_ready() {
        const bool go = rnd_bool() != 0;
        if (go) {
            fmt::println(" (executed)");
        } else {
            fmt::println(" (suspended)");
        }
        return go;
    }

    void await_suspend(std::coroutine_handle<> h) {
        thd.get() = std::jthread([h] { h.resume(); });
    }

    void await_resume() {}
};

struct job2 {
    inline static std::size_t job_count;

    job2() {
        ++job_count;
    }

    struct promise_type {
        job2 get_return_object() {
            return {};
        }

        std::suspend_never initial_suspend() {
            fmt::println("    Job {} prepared on thread {}", job_count, std::this_thread::get_id());
            return {};
        }

        std::suspend_never final_suspend() noexcept {
            fmt::println("    Job {} finished on thread {}", job_count, std::this_thread::get_id());
            return {};
        }

        void return_void() {}

        void unhandled_exception() {
            std::abort();
        }
    };
};

inline job2 perform_job2(std::jthread& th) {
    co_await my_awaitable(th);
}

} // namespace coro
