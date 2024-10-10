// Kingsley Chen

#pragma once

#include <atomic>
#include <coroutine>
#include <type_traits>

#include "thread_pool/task.h"

namespace coro {

//
// wait_once event
//

class wait_once {
public:
    void signal() noexcept {
        flag_.test_and_set();
        flag_.notify_all();
    }

    void wait() noexcept {
        flag_.wait(false);
    }

private:
    // Default ctor will initialize to clear state, since C++ 20.
    std::atomic_flag flag_;
};

// std::atomic_flag itself is neither copyable nor movable.
static_assert(!std::is_copy_constructible_v<wait_once> &&
              !std::is_move_constructible_v<wait_once>);

//
// sync_wait_task
//

class [[nodiscard]] sync_wait_task {
    struct promise {
        sync_wait_task get_return_object() noexcept {
            return sync_wait_task(std::coroutine_handle<promise>::from_promise(*this));
        }

        // We need to associate with a `wait_once` before execution of coroutine body.
        [[nodiscard]] std::suspend_always initial_suspend() const noexcept {
            (void)this;
            return {};
        }

        // Need to signal event after sync_wait_task has completed execution.
        [[nodiscard]] auto final_suspend() const noexcept {
            (void)this;
            struct final_awaiter {
                [[nodiscard]] constexpr bool await_ready() const noexcept {
                    (void)this;
                    return false;
                }

                void await_suspend(std::coroutine_handle<promise> coro) const noexcept {
                    (void)this;
                    if (auto* event = coro.promise().once_event; event) {
                        event->signal();
                    }
                }

                constexpr void await_resume() const noexcept {}
            };
            return final_awaiter{};
        }

        void return_void() const noexcept {}

        // Simplified.
        void unhandled_exception() const noexcept {
            (void)this;
            std::abort();
        }

        wait_once* once_event{nullptr};
    };

public:
    using promise_type = promise;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit sync_wait_task(handle_type coro_handle) noexcept
        : coro_(coro_handle) {}

    ~sync_wait_task() {
        if (coro_) {
            coro_.destroy();
        }
    }

    sync_wait_task(const sync_wait_task&) = default;
    sync_wait_task(sync_wait_task&&) = default;
    sync_wait_task& operator=(const sync_wait_task&) = default;
    sync_wait_task& operator=(sync_wait_task&&) = default;

    void run(wait_once& event) {
        coro_.promise().once_event = &event;
        coro_.resume();
    }

private:
    handle_type coro_{nullptr};
};

inline sync_wait_task make_sync_wait_task(
        task& t) { // NOLINT(*-avoid-reference-coroutine-parameters)
    // Make this awaiting coroutine as continuation of `task`, and after the completion of task
    // this awaiting coroutine will be resumed and hence the final_suspend() of sync_wait_task will
    // signal the event.
    co_await t;
}

inline void sync_wait(task& t) {
    wait_once event;
    auto wait_task = make_sync_wait_task(t);
    wait_task.run(event); // resume sync_wait_task
    event.wait();
}

} // namespace coro
