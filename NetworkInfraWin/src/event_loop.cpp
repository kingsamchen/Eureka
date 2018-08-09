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
      owner_thread_id_(GetCurrentThreadId()),
      executing_pending_task_(false)
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

        ProcessPendingTasks();
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

void EventLoop::RunTaskInLoop(const Task& task)
{
    if (BelongsToCurrentThread()) {
        task();
    } else {
        QueueTask(task);
    }
}

void EventLoop::QueueTask(const Task& task)
{
    {
        std::lock_guard<std::mutex> lock(task_list_mutex_);
        pending_tasks_.push_back(task);
    }

    if (!BelongsToCurrentThread() || executing_pending_task_) {
        event_pump_.Wakeup();
    }
}

void EventLoop::ProcessPendingTasks()
{
    executing_pending_task_ = true;

    std::vector<Task> pending_tasks;

    {
        std::lock_guard<std::mutex> lock(task_list_mutex_);
        pending_tasks.swap(pending_tasks_);
    }

    for (auto& task : pending_tasks) {
        task();
    }

    executing_pending_task_ = false;
}

}   // namespace network
