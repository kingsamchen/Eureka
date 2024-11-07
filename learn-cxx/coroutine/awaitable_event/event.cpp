// Kingsley Chen

#include "awaitable_event/event.h"

#include <cassert>

#include <sys/eventfd.h>

#include "awaitable_event/event_poller.h"

namespace coro {
namespace {

esl::unique_fd create_eventfd() {
    auto evfd = esl::wrap_unique_fd(::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK));
    if (!evfd) {
        throw std::system_error(errno, std::system_category(), "create eventfd failure");
    }
    return evfd;
}

} // namespace

waitable_event::waitable_event()
    : evfd_(create_eventfd()) {}

void waitable_event::wait(std::coroutine_handle<> coro) {
    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.ptr = coro.address();
    global_event_poller().add_event(evfd_.get(), ev);
}

void waitable_event::signal() {
    const bool prev_signaled = signaled_.exchange(true, std::memory_order_acq_rel);
    if (prev_signaled) {
        return;
    }

    // Buggy: if failed here, the event should be back to previous state if was in unsignaled.
    // but some other new awaits may have continued during this window.
    if (eventfd_write(evfd_.get(), 1) < 0) {
        throw std::system_error(errno, std::system_category(), "write to eventfd");
    }
}

tl::generator<std::coroutine_handle<>> signaled_events() {
    auto signaled = global_event_poller().poll(std::chrono::milliseconds{0});
    for (const auto& ev : signaled) {
        // Cannot co_yield a temporary.
        auto coro_handle = std::coroutine_handle<>::from_address(ev.data.ptr);
        assert(!coro_handle.done());
        co_yield coro_handle;
    }
}

} // namespace coro
