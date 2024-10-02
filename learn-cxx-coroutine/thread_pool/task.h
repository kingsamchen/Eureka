// Kingsley Chen

#pragma once

#include <coroutine>
#include <exception>

#include "fmt/format.h"

namespace coro {

class [[nodiscard]] task {
    struct promise {
        struct final_awaiter {
            // Must suspend, because we need to keep coroutine frame alive until we cleanup it
            // manually.
            [[nodiscard]] constexpr bool await_ready() const noexcept {
                (void)this;
                return false;
            }

            constexpr void await_resume() const noexcept {}

            // Returns a coroutine handle, and this handle will be resumed and thus the continuation
            // will be executed.
            [[nodiscard]] std::coroutine_handle<> await_suspend(
                    std::coroutine_handle<promise> coro) const noexcept {
                (void)this;
                return coro.promise().continuation_;
            }
        };

        // Cannot be `const` because `from_promise()` requires `Promise&`.
        task get_return_object() noexcept {
            return task(std::coroutine_handle<promise>::from_promise(*this));
        }

        // We need to set continuation before starting execution of the coroutine.
        [[nodiscard]] std::suspend_always initial_suspend() const noexcept {
            (void)this;
            return {};
        }

        // Changed to our final_awaiter and we now have to cleanup coroutine frame on our own.
        [[nodiscard]] final_awaiter final_suspend() const noexcept {
            (void)this;
            return final_awaiter{};
        }

        void return_void() const noexcept {}

        void unhandled_exception() noexcept {
            exception_ = std::current_exception();
        }

        void rethrow_if_exception() const {
            if (exception_) {
                std::rethrow_exception(exception_);
            }
        }

        void set_continuation(std::coroutine_handle<> continuation) noexcept {
            continuation_ = continuation;
        }

        std::coroutine_handle<> continuation_{std::noop_coroutine()};
        std::exception_ptr exception_;
    };

public:
    using promise_type = promise;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit task(handle_type coro_handle) noexcept
        : coro_handle_(coro_handle) {}

    ~task() {
        if (coro_handle_) {
            coro_handle_.destroy();
        }
    }

    task(const task&) = default;
    task(task&&) = default;
    task& operator=(const task&) = default;
    task& operator=(task&&) = default;

    // Force resume, for understanding inner working details.
    void resume() {
        coro_handle_.resume();
    }

    // To achive `co_await task`, and
    // want to keep task awaiter internal details, so lets define operator co_await to bridge the
    // gap.
    auto operator co_await() noexcept {
        struct awaiter {
            [[nodiscard]] bool await_ready() const noexcept {
                return !task_coro || task_coro.done();
            }

            // Resume task_coro and task_coro's promise's final_awaiter will subsequently resume
            // awaiting_coro.
            [[nodiscard]] std::coroutine_handle<> await_suspend(
                    std::coroutine_handle<> awaiting_coro) noexcept {
                (void)this;
                task_coro.promise().set_continuation(awaiting_coro);
                return task_coro;
            }

            void await_resume() const noexcept {}

            handle_type task_coro;
        };

        return awaiter{coro_handle_};
    }

private:
    handle_type coro_handle_{nullptr};
};

} // namespace coro
