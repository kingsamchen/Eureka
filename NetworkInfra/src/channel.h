/*
 @ 0xCCCCCCCC
*/

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include <poll.h>

#include "kbase/basic_macros.h"

namespace network {

class EventLoop;

class Channel {
public:
    using EventHandler = std::function<void()>;

    enum IOEvent : int {
        None = 0,
        Read = POLLIN | POLLPRI,
        Write = POLLOUT
    };

    Channel(EventLoop* loop, int fd);

    ~Channel();

    DISALLOW_COPY(Channel);

    DISALLOW_MOVE(Channel);

    void HandleEvents();

    bool BelongsToEventLoop(EventLoop* loop) const noexcept
    {
        return loop_ == loop;
    }

    void EnableReading()
    {
        events_ |= IOEvent::Read;
        Update();
    }

    void DisableAll()
    {
        events_ = IOEvent::None;
        Update();
    }

    void set_read_handler(const EventHandler& handler)
    {
        read_handler_ = handler;
    }

    void set_write_handler(const EventHandler& handler)
    {
        write_handler_ = handler;
    }

    void set_close_handler(const EventHandler& handler)
    {
        close_handler_ = handler;
    }

    void set_error_handler(const EventHandler& handler)
    {
        error_handler_ = handler;
    }

    int fd() const noexcept
    {
        return fd_;
    }

    bool none_event() const noexcept
    {
        return events_ == IOEvent::None;
    }

    int events() const noexcept
    {
        return events_;
    }

    void set_received_events(int events) noexcept
    {
        received_events_ = events;
    }

    int poller_info_index() const noexcept
    {
        return index_;
    }

    void set_poller_info_index(int index) noexcept
    {
        index_ = index;
    }

private:
    void Update();

private:
    EventLoop* loop_;
    int fd_;
    int events_;
    int received_events_;
    int index_;
    bool handling_events_;
    EventHandler read_handler_;
    EventHandler write_handler_;
    EventHandler close_handler_;
    EventHandler error_handler_;
};

}   // namespace work

#endif  // CHANNEL_H_
