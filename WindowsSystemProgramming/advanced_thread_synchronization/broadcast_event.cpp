/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <Windows.h>

#include "kbase/error_exception_util.h"
#include "kbase/scoped_handle.h"

namespace {

kbase::ScopedWinHandle g_event(CreateEventW(nullptr, TRUE, FALSE, nullptr));

std::mutex io_mutex;

void WorkerFunc(int id)
{
    {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "Worker #" << id << "\n";
    }

    WaitForSingleObject(g_event.get(), INFINITE);

    {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "Worker #" << id << " wake up!" << std::endl;
    }
}

}   // namespace

void TestBroadcastOnEvent()
{
    ENSURE(CHECK, !!g_event).Require();

    std::vector<std::thread> workers;
    for (int i = 0; i < 3; ++i) {
        workers.push_back(std::thread(WorkerFunc, i));
    }

    {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "Main thread enters into sleep!" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "Main thread wake up!" << std::endl;
    }

    SetEvent(g_event.get());

    for (auto& th : workers) {
        th.join();
    }
}
