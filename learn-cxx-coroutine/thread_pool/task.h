// Kingsley Chen

#pragma once

#include <coroutine>
#include <exception>

namespace coro {

class [[nodiscard]] task {
    struct promise {
        // Cannot be `const` because `from_promise()` requires `Promise&`.
        task get_return_object() noexcept {
            return task(std::coroutine_handle<promise>::from_promise(*this));
        }

        [[nodiscard]] std::suspend_never initial_suspend() const noexcept {
            (void)this;
            return {};
        }

        [[nodiscard]] std::suspend_never final_suspend() const noexcept {
            (void)this;
            return {};
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

        std::exception_ptr exception_;
    };

public:
    using promise_type = promise;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit task(handle_type coro_handle) noexcept
        : coro_handle_(coro_handle) {}

private:
    handle_type coro_handle_{nullptr};
};

} // namespace coro
