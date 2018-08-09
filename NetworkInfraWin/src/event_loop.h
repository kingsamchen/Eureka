/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <functional>
#include <mutex>
#include <vector>

#include "kbase/basic_macros.h"

#include "event_pump.h"

namespace network {

class Notifier;

class EventLoop {
public:
    using Task = std::function<void()>;

    EventLoop();

    ~EventLoop();

    DISALLOW_COPY(EventLoop);

    DISALLOW_MOVE(EventLoop);

    // Get the pointer to the EventLoop for current thread.
    static EventLoop* current() noexcept;

    void Run();

    void Quit();

    void RunTaskInLoop(const Task& task);

    void QueueTask(const Task& task);

    // Returns true if the EventLoop is owned by current thread;
    // Returns false otherwise.
    bool BelongsToCurrentThread() const noexcept;

    void SubscribeNotifier(Notifier* notifier);

private:
    void ProcessPendingTasks();

private:
    bool is_running_;
    unsigned long owner_thread_id_;
    EventPump event_pump_;
    bool executing_pending_task_;
    std::mutex task_list_mutex_;
    std::vector<Task> pending_tasks_;
};

}   // namespace network

#endif  // EVENT_LOOP_H_
