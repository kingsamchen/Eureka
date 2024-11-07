// Kingsley Chen

#include "awaitable_event/event_poller.h"

#include <sys/epoll.h>
#include <unistd.h>

namespace coro {
namespace {

esl::unique_fd create_epfd() {
    auto epfd = esl::wrap_unique_fd(::epoll_create1(EPOLL_CLOEXEC));
    if (!epfd) {
        throw std::system_error(errno, std::system_category(), "create epfd failure");
    }
    return epfd;
}

} // namespace

event_poller::event_poller()
    : epfd_(create_epfd()) {
    const int init_events_cap = getpagesize() * 2;
    io_events_.resize(static_cast<std::size_t>(init_events_cap));
}

// We don't have extra bookkeeping for whether the event was already added, so lets do the check
// in flight.
void event_poller::add_event(int fd, epoll_event& event) {
    if (::epoll_ctl(epfd_.get(), EPOLL_CTL_ADD, fd, &event) == 0) {
        return;
    }

    if (errno == EEXIST) {
        if (::epoll_ctl(epfd_.get(), EPOLL_CTL_MOD, fd, &event) == 0) {
            return;
        }
    }

    throw std::system_error(errno, std::system_category(), "add epoll event failure");
}

void event_poller::remove_event(int fd) {
    // We don't target kernels before 2.6.9, so we can pass nullptr as the last argument.
    // See https://man7.org/linux/man-pages/man2/epoll_ctl.2.html.
    if (::epoll_ctl(epfd_.get(), EPOLL_CTL_DEL, fd, nullptr) == -1) {
        throw std::system_error(errno, std::system_category(), "remove epoll event failure");
    }
}

tl::generator<epoll_event> event_poller::poll(std::chrono::milliseconds timeout) {
    const int count = ::epoll_wait(epfd_.get(),
                                   io_events_.data(),
                                   static_cast<int>(io_events_.size()),
                                   static_cast<int>(timeout.count()));
    if (count == -1) {
        throw std::system_error(errno, std::system_category(), "poll events");
    }

    for (std::size_t i = 0; i < static_cast<std::size_t>(count); ++i) {
        co_yield io_events_[i];
    }
}

event_poller& global_event_poller() {
    static event_poller poller;
    return poller;
}

} // namespace coro
