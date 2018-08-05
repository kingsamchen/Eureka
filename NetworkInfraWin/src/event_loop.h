/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include "kbase/basic_macros.h"

namespace network {

class EventLoop {
public:
    EventLoop();

    ~EventLoop();

    DISALLOW_COPY(EventLoop);

    DISALLOW_MOVE(EventLoop);

    // Get the pointer to the EventLoop for current thread.
    static EventLoop* current() noexcept;

    void Run();

    void Quit();

    // Returns true if the EventLoop is owned by current thread;
    // Returns false otherwise.
    bool BelongsToCurrentThread() const noexcept;

private:
    bool is_running_;
    unsigned long owner_thread_id_;
};

}   // namespace network

#endif  // EVENT_LOOP_H_
