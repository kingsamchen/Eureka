/*
 @ 0xCCCCCCCC
*/

#include "event_loop.h"

#include <Windows.h>

#include "kbase/error_exception_util.h"

namespace network {

thread_local EventLoop* tls_loop_in_thread {nullptr};

EventLoop::EventLoop()
    : is_running_(false),
      owner_thread_id_(GetCurrentThreadId())
{
    ENSURE(CHECK, tls_loop_in_thread == nullptr).Require();
    tls_loop_in_thread = this;
}

EventLoop::~EventLoop()
{
    ENSURE(CHECK, !is_running_).Require();

    tls_loop_in_thread = nullptr;
}

void EventLoop::Run()
{
    is_running_ = true;
    while (is_running_) {
        // TODO: get notifiers from event-pump
    }
}

void EventLoop::Quit()
{
    // FIXME: use atomic
    // TODO: wakeup
    is_running_ = false;
}

// static
EventLoop* EventLoop::current() noexcept
{
    return tls_loop_in_thread;
}

bool EventLoop::BelongsToCurrentThread() const noexcept
{
    return owner_thread_id_ == GetCurrentThreadId();
}

}   // namespace network
