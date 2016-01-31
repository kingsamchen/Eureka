/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EUREKA_ACTIVE_THREAD_ACTIVE_THREAD_H_
#define EUREKA_ACTIVE_THREAD_ACTIVE_THREAD_H_

#include <functional>
#include <memory>

#include "blocking_queue.h"

namespace std {
class thread;
}

using Task = std::function<void()>;

class ActiveThread {
public:
    ActiveThread();

    // TODO: dtor

    DISALLOW_COPY(ActiveThread);

    DISALLOW_MOVE(ActiveThread);

private:
    void Run();

private:
    BlockingQueue<Task> task_queue_;
    bool done_;
    std::unique_ptr<std::thread> thread_;
};

#endif  // EUREKA_ACTIVE_THREAD_ACTIVE_THREAD_H_
