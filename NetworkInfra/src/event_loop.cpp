/*
 @ 0xCCCCCCCC
*/

#include "event_loop.h"

#include <cstdlib>
#include <vector>

#include <poll.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "kbase/error_exception_util.h"

#include "channel.h"

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
      owner_thread_id_(GetCurrentThreadID())
{
    ENSURE(CHECK, tls_loop_in_thread == nullptr).Require();

    tls_loop_in_thread = this;

    poller_ = std::make_unique<Poller>(this);
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

    std::vector<Channel*> active_channels;

    while (is_running_) {
        poller_->Poll(active_channels, std::chrono::seconds(10));

        for (auto& channel : active_channels) {
            channel->HandleEvents();
        }

        active_channels.clear();
    }

    printf("Finish the eventloop\n");
}

void EventLoop::Quit()
{
    is_running_ = false;
}

void EventLoop::UpdateChannel(Channel* channel)
{
    ENSURE(CHECK, channel->BelongsToEventLoop(this)).Require();
    poller_->UpdateChannel(channel);
}

bool EventLoop::BelongsToCurrentThread() const
{
    return owner_thread_id_ == GetCurrentThreadID();
}

// static
EventLoop* EventLoop::current()
{
    return tls_loop_in_thread;
}

}   // namespace network
