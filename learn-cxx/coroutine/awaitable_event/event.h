// Kingsley Chen

#pragma once

#include <atomic>
#include <coroutine>

#include "esl/unique_handle.h"
#include "tl/generator.hpp"

namespace coro {

class waitable_event {
public:
    waitable_event();

    ~waitable_event() = default;

    waitable_event(const waitable_event&) = delete;

    waitable_event(waitable_event&&) = delete;

    waitable_event& operator=(const waitable_event&) = delete;

    waitable_event& operator=(waitable_event&&) = delete;

    auto operator co_await() noexcept {
        struct awaiter {
            // If the event is already signaled, no need to await.
            [[nodiscard]] bool await_ready() const noexcept {
                return event->signaled_.load(std::memory_order_acquire);
            }

            // Reset to unsignaled for next await.
            void await_resume() const noexcept {
                event->signaled_.store(false, std::memory_order_release);
            }

            void await_suspend(std::coroutine_handle<> awaiting_coro) const {
                event->wait(awaiting_coro);
            }

            waitable_event* event;
        };

        return awaiter{this};
    }

    void signal();

private:
    void wait(std::coroutine_handle<> coro);

    esl::unique_fd evfd_;
    std::atomic<bool> signaled_{false};
};

tl::generator<std::coroutine_handle<>> signaled_events();

} // namespace coro
