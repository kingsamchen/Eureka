/*
 @ 0xCCCCCCCC
*/

#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include <sys/types.h>

#include "kbase/basic_macros.h"

#include "poller.h"
#include "timer_queue.h"

namespace network {

class Channel;

class EventLoop {
public:
    using Task = std::function<void()>;

    EventLoop();

    ~EventLoop();

    DISALLOW_COPY(EventLoop);

    DISALLOW_MOVE(EventLoop);

    void Run();

    // If the function is called on loop thread, execute the task immediately;
    // otherwise, queue up the task and move to loop thread to run.
    void RunTask(const Task& task);

    void RunTaskAt(chrono::system_clock::time_point when, const Timer::EventHandler& handler);

    void RunTaskAfter(chrono::microseconds, const Timer::EventHandler& handler);

    // Wake up if we are queuing from another thread or are queuing during execution of pending
    // tasks. If the latter being the case, we need to continue running pending tasks. Don't
    // get stuck at polling.
    void QueueTask(const Task& task);

    void Quit();

    void UpdateChannel(Channel* channel);

    void RemoveChannel(Channel* channel);

    bool BelongsToCurrentThread() const;

    static EventLoop* current();

private:
    void ProcessPendingTasks();

    void Wakeup();

    void OnWakeup();

private:
    bool is_running_;
    pid_t owner_thread_id_;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<TimerQueue> timer_queue_;
    int wakeup_fd_;
    Channel wakeup_channel_;
    bool executing_pending_task_;
    std::mutex task_list_mutex_;
    std::vector<Task> pending_tasks_;
};

}   // namespace network

#endif //EVENT_LOOP_H_
