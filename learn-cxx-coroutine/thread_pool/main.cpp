// Kingsley Chen

#include <chrono>
#include <thread>

#include "fmt/format.h"
#include "fmt/std.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

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

TEST_CASE("schedule") {
    auto run_async_print = [](coro::thread_pool& pool) -> coro::task {
        fmt::println("before run async print on {}", std::this_thread::get_id());
        co_await pool.schedule();
        fmt::println("this is a hello from thread: {}", std::this_thread::get_id());
    };

    fmt::println("The main thread id is: {}", std::this_thread::get_id());

    coro::thread_pool pool(4);
    [[maybe_unused]] auto t = run_async_print(pool);
    fmt::println("wait for coroutine to resume");

    std::this_thread::sleep_for(std::chrono::seconds(5));
}
