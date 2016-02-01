/*
 @ 0xCCCCCCCC
*/

#include "active_thread.h"

#include <thread>

ActiveThread::ActiveThread()
    : done_(false)
{
    thread_ = std::make_unique<std::thread>(&ActiveThread::Run, this);
}

ActiveThread::~ActiveThread()
{
    auto quit_task = [this] { done_ = true; };
    PostTask(std::move(quit_task));
    thread_->join();
}

void ActiveThread::Run()
{
    while (!done_) {
        Task task;
        task_queue_.Dequeue(&task);
        task();
    }
}

void ActiveThread::PostTask(Task&& task)
{
    task_queue_.Enqueue(task);
}