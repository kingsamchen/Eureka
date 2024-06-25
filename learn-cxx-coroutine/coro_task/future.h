//
// Kingsley Chen
//

#pragma once

#include <coroutine>
#include <memory>

namespace coro {

template<typename T>
class future {
public:
    explicit future(std::shared_ptr<T> v)
        : value_(std::move(v)) {}

    const T& get() {
        return *value_;
    }

    struct promise_type {
        std::shared_ptr<T> ptr = std::make_shared<T>();

        future<T> get_return_object() {
            return future(ptr);
        }

        void return_value(const T& v) {
            *ptr = v;
        }

        std::suspend_never initial_suspend() noexcept {
            return {};
        }

        std::suspend_never final_suspend() noexcept {
            return {};
        }

        void unhandled_exception() {
            std::abort();
        }
    };

private:
    std::shared_ptr<T> value_;
};

} // namespace coro
