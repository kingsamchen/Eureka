// Kingsley Chen

#include <chrono>
#include <thread>

#include "fmt/format.h"
#include "fmt/std.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "thread_pool/sync_wait.h"
#include "thread_pool/task.h"
#include "thread_pool/thread_pool.h"

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

TEST_CASE("Start and stop thread-pool") {
    coro::thread_pool pool(4);
    std::this_thread::sleep_for(std::chrono::seconds{2});
}

TEST_CASE("wait_once event") {
    coro::wait_once wo;
    std::jthread thd([&wo] {
        fmt::println("on thd");
        std::this_thread::sleep_for(std::chrono::seconds{3});
        wo.signal();
    });

    fmt::println("wait on event");
    wo.wait();
}

// Output
// worker-139866389083712 starts running
// worker-139866380691008 starts running
// task::coro_handle_ addr=0x508000000020
// inside the run_task
// run the run_task inside pool
// task::coro_handle_ addr=0x506000003020
// task_coro addr=0x506000003020 awaiting_coro addr=0x508000000020
// inside the sub_task
// run sub_task inside pool
// leave the sub_task
// task promise coro addr=0x506000003020 continuation=0x508000000020
// leave the run_task
// task promise coro addr=0x508000000020 continuation=0x558c83a77b20
// -EOF-
// worker-139866389083712 stopped
// worker-139866380691008 stopped
TEST_CASE("understanding task continuation") {
    coro::thread_pool pool(2);
    std::this_thread::sleep_for(std::chrono::seconds{2});

    auto run_task = [&pool]() -> coro::task { // NOLINT
        fmt::println("inside the run_task");
        co_await pool.schedule();
        fmt::println("run the run_task inside pool");
        auto sub_task = [&pool]() -> coro::task { // NOLINT
            fmt::println("inside the sub_task");
            co_await pool.schedule();
            fmt::println("run sub_task inside pool");
            std::this_thread::sleep_for(std::chrono::seconds{2});
            fmt::println("leave the sub_task");
        };
        // sub_task::operator co_await ->
        //  sub_task::coro_handle.promise.set_continuation(run_task::coro_handle)
        // and run_task::coro_handle is the awaiting coro_handled passed to
        // sub_task operator co_await's internal awaiter.
        co_await sub_task();
        fmt::println("leave the run_task");
    };

    auto t = run_task();
    t.resume();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    fmt::println("-EOF-");
}

TEST_CASE("schedule and wait") {
    auto run_async_print = [](coro::thread_pool& pool) -> coro::task { // NOLINT
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fmt::println("before run async print on {}", std::this_thread::get_id());
        co_await pool.schedule();
        fmt::println("this is a hello from thread: {}", std::this_thread::get_id());
    };

    fmt::println("The main thread id is: {}", std::this_thread::get_id());

    coro::thread_pool pool(4);
    [[maybe_unused]] auto t = run_async_print(pool);
    fmt::println("wait for coroutine to resume");

    // body of run_async_print starts executing by sync_wait.
    coro::sync_wait(t);
}
