/*
 @ 0xCCCCCCCC
*/

#include "event_loop.h"

#include <cstdlib>

#include <poll.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "kbase/error_exception_util.h"

namespace {

pid_t GetCurrentThreadID()
{
    return static_cast<pid_t>(syscall(SYS_gettid));
}

}   // namespace

namespace network {

thread_local EventLoop* tls_loop_in_thread {nullptr};

EventLoop::EventLoop()
    : is_running_(false),
      associated_thread_id_(GetCurrentThreadID())
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
    ENSURE(CHECK, !is_running_).Require();

    is_running_ = true;

    printf("Running the eventloop\n");

    poll(nullptr, 0, 5000);

    is_running_ = false;

    printf("Finish the eventloop\n");
}

// static
EventLoop* EventLoop::current()
{
    return tls_loop_in_thread;
}

}   // namespace network
