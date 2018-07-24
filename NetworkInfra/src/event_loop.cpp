/*
 @ 0xCCCCCCCC
*/

#include "event_loop.h"

#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <unistd.h>


#include "kbase/error_exception_util.h"

namespace {

pid_t GetCurrentThreadID()
{
    // TODO: cache tid in tls.
    return static_cast<pid_t>(syscall(SYS_gettid));
}

int CreateWakeupFD()
{
    int fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    ENSURE(CHECK, fd != -1)(errno).Require();
    return fd;
}

}   // namespace

namespace network {

thread_local EventLoop* tls_loop_in_thread {nullptr};

EventLoop::EventLoop()
    : is_running_(false),
      owner_thread_id_(GetCurrentThreadID()),
      poller_(std::make_unique<Poller>(this)),
      timer_queue_(std::make_unique<TimerQueue>(this)),
      wakeup_fd_(CreateWakeupFD()),
      wakeup_channel_(this, wakeup_fd_),
      executing_pending_task_(false)
{
    ENSURE(CHECK, tls_loop_in_thread == nullptr).Require();

    tls_loop_in_thread = this;

    wakeup_channel_.set_read_handler([this] { OnWakeup(); });
    wakeup_channel_.EnableReading();
}

EventLoop::~EventLoop()
{
    ENSURE(CHECK, !is_running_).Require();

    close(wakeup_fd_);

    tls_loop_in_thread = nullptr;
}

void EventLoop::Run()
{
    ENSURE(CHECK, !is_running_).Require();

    is_running_ = true;

    std::vector<Channel*> active_channels;

    while (is_running_) {
        poller_->Poll(active_channels, std::chrono::seconds(10));

        for (auto& channel : active_channels) {
            channel->HandleEvents();
        }

        active_channels.clear();

        ProcessPendingTasks();
    }
}

void EventLoop::RunTask(const Task& task)
{
    if (BelongsToCurrentThread()) {
        task();
    } else {
        QueueTask(task);
    }
}

void EventLoop::RunTaskAt(chrono::system_clock::time_point when, const Timer::EventHandler& handler)
{
    Task task(std::bind(&TimerQueue::AddTimer,
                        timer_queue_.get(),
                        handler,
                        when,
                        chrono::microseconds(0)));
    RunTask(task);
}

void EventLoop::RunTaskAfter(chrono::microseconds delay, const Timer::EventHandler& handler)
{
    Task task(std::bind(&TimerQueue::AddTimer,
                        timer_queue_.get(),
                        handler,
                        chrono::system_clock::now() + delay,
                        chrono::microseconds(0)));
    RunTask(task);
}

void EventLoop::Quit()
{
    is_running_ = false;
    if (!BelongsToCurrentThread()) {
        Wakeup();
    }
}

void EventLoop::UpdateChannel(Channel* channel)
{
    ENSURE(CHECK, channel->BelongsToEventLoop(this)).Require();
    poller_->UpdateChannel(channel);
}

void EventLoop::RemoveChannel(Channel* channel)
{
    ENSURE(CHECK, channel->BelongsToEventLoop(this)).Require();
    poller_->RemoveChannel(channel);
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

void EventLoop::QueueTask(const Task& task)
{
    {
        std::lock_guard<std::mutex> lock(task_list_mutex_);
        pending_tasks_.push_back(task);
    }

    if (!BelongsToCurrentThread() || executing_pending_task_) {
        Wakeup();
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

void EventLoop::Wakeup()
{
    uint64_t data = 1;
    ssize_t n = write(wakeup_fd_, &data, sizeof(data));
    ENSURE(CHECK, n == sizeof(data))(n)(sizeof(data)).Require();
}

void EventLoop::OnWakeup()
{
    uint64_t data;
    ssize_t n = read(wakeup_fd_, &data, sizeof(data));
    ENSURE(CHECK, n == sizeof(data))(n)(sizeof(data)).Require();
}

}   // namespace network
