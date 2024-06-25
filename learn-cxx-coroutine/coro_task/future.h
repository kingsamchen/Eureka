//
// Kingsley Chen
//

#pragma once

#include <coroutine>
#include <cstdlib>

namespace coro {

template<typename T>
class future {
    struct promise {
        T value;

        future<T> get_return_object() {
            return future(std::coroutine_handle<promise>::from_promise(*this));
        }

        void return_value(const T& v) {
            value = v;
        }

        std::suspend_always initial_suspend() noexcept {
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            return {};
        }

        void unhandled_exception() {
            std::abort();
        }
    };

public:
    using promise_type = promise;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit future(handle_type handle) noexcept
        : handle_(handle) {}

    ~future() {
        if (handle_) {
            handle_.destroy();
        }
    }

    // Disallow copy and move(so far)
    future(const future&) = delete;
    future(future&&) = delete;
    future& operator=(const future&) = delete;
    future& operator=(future&&) = delete;

    T get() {
        handle_.resume();
        return handle_.promise().value;
    }

private:
    handle_type handle_;
};

} // namespace coro
