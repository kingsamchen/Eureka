// Kingsley Chen

#include <chrono>
#include <vector>

#include <sys/epoll.h>

#include "tl/generator.hpp"

#include "esl/unique_handle.h"

namespace coro {

class event_poller {
public:
    // Throws std::system_error if failed to create epoll fd.
    event_poller();

    ~event_poller() = default;

    event_poller(const event_poller&) = delete;

    event_poller(event_poller&&) = delete;

    event_poller& operator=(const event_poller&) = delete;

    event_poller& operator=(event_poller&&) = delete;

    // If fd was not added, add the fd/event to epoll monitoring list
    // Otherwise, update the event properties for the fd.
    // Throws std::system_error on epoll_ctl failure.
    void add_event(int fd, epoll_event& event);

    // Throws std::system_error on epoll_ctl failure.
    void remove_event(int fd);

    tl::generator<epoll_event> poll(std::chrono::milliseconds timeout);

private:
    esl::unique_fd epfd_;
    std::vector<epoll_event> io_events_;
};

event_poller& global_event_poller();

} // namespace coro
