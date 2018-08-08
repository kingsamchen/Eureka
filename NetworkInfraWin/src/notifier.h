/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef NOTIFIER_H_
#define NOTIFIER_H_

#include <functional>

#include <winsock2.h>

#include "kbase/basic_macros.h"

namespace network {

class EventLoop;
struct IOContext;

class Notifier {
public:
    using EventHandler = std::function<void(IOContext*)>;

    Notifier(EventLoop* loop, SOCKET socket);

    ~Notifier() = default;

    DISALLOW_COPY(Notifier);

    DISALLOW_MOVE(Notifier);

    void SetReadCompleteEvent(const EventHandler& handler)
    {
        read_complete_event_ = handler;
    }

    void SetWriteCompleteEvent(const EventHandler& handler)
    {
        write_complete_event_ = handler;
    }

    void HandleCompletionEvent(IOContext* io_context) const;

    SOCKET socket() const noexcept
    {
        return socket_;
    }

    void EnableNotification();

private:
    EventLoop* loop_;
    SOCKET socket_;
    EventHandler read_complete_event_;
    EventHandler write_complete_event_;
};

}   // namespace network

#endif  // NOTIFIER_H_
