// Kingsley Chen

#pragma once

#include <coroutine>
#include <cstdlib>

namespace coro {

struct no_return {
    struct promise_type {
        [[nodiscard]] no_return get_return_object() const noexcept {
            return no_return{};
        }

        [[nodiscard]] std::suspend_never initial_suspend() const noexcept {
            return {};
        }

        [[nodiscard]] std::suspend_never final_suspend() const noexcept {
            return {};
        }

        void return_void() const noexcept {}

        void unhandled_exception() const noexcept {
            std::abort();
        }
    };
};

} // namespace coro
