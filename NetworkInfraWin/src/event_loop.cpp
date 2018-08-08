/*
 @ 0xCCCCCCCC
*/

#include "event_loop.h"

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

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

    std::vector<std::pair<Notifier*, IOContext*>> active_notifiers;
    while (is_running_) {
        event_pump_.Pump(std::chrono::seconds(10), active_notifiers);

        LOG(INFO) << "Returns from EventPump with " << active_notifiers.size() << " active events";

        for (auto& p : active_notifiers) {
            p.first->HandleCompletionEvent(p.second);
        }

        active_notifiers.clear();
    }
}

void EventLoop::Quit()
{
    // FIXME: use atomic
    // FIXME: consider thread-safety
    is_running_ = false;
    if (!BelongsToCurrentThread()) {
        event_pump_.Wakeup();
    }
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

void EventLoop::SubscribeNotifier(Notifier* notifier)
{
    event_pump_.SubscribeNotifier(notifier);
}

}   // namespace network
