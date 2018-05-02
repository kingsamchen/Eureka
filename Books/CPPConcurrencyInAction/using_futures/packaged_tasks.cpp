/*
 @ 0xCCCCCCCC
*/

#include <cstdio>
#include <deque>
#include <future>
#include <mutex>
#include <random>
#include <thread>

#include "kbase/scope_guard.h"

using Task = std::packaged_task<void()>;

std::mutex queue_mutex;
std::deque<Task> task_queue;
std::atomic<bool> is_done {false};

template<typename F>
std::future<void> PostTaskToMainThread(F fn)
{
    Task task(fn);
    auto future = task.get_future();

    std::lock_guard<std::mutex> lock(queue_mutex);
    task_queue.push_back(std::move(task));

    return future;
}

void RunMainThreadLoop()
{
    while (!is_done.load()) {
        Task task;

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (task_queue.empty()) {
                continue;
            }

            task = std::move(task_queue.back());
            task_queue.pop_back();
        }

        task();
    }

    printf("Main loop end...");
}

void TestPackagedTasks()
{
    std::thread worker([]() {
        std::default_random_engine engine(std::random_device{}());
        std::uniform_int_distribution<> rnd(1, 3);
        for (int i = 0; i < 5; ++i) {
            PostTaskToMainThread([i]() {
                printf("Execute task %d\n", i + 1);
            });

            printf("worker sleep\n");
            std::this_thread::sleep_for(std::chrono::seconds(rnd(engine)));
        }

        printf("worker exit...\n");

        PostTaskToMainThread([]() {
            is_done.store(true);
        });
    });

    ON_SCOPE_EXIT { worker.join(); };

    RunMainThreadLoop();
}
