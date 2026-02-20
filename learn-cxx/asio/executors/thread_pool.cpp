//
// 0xCCCCCCCC
//

#include <latch>
#include <mutex>
#include <thread>
#include <unordered_set>

#include <asio.hpp>
#include <fmt/core.h>
#include <fmt/std.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("Get thread-id of all threads in the pool") {
    asio::thread_pool pool(4);

    std::unordered_set<std::thread::id> tids;
    std::mutex mtx;
    std::latch latch(4);

    for (int i = 0; i < 4; ++i) {
        asio::post(pool, [&latch, &tids, &mtx] {
            {
                std::lock_guard lock(mtx);
                tids.insert(std::this_thread::get_id());
            }

            fmt::println("thread/{} arrive and wait", std::this_thread::get_id());
            latch.arrive_and_wait();
            fmt::println("thread/{} complete", std::this_thread::get_id());
        });
    }

    latch.wait();
    pool.join();

    CHECK_EQ(tids.size(), 4);
}

TEST_CASE("Switch between main thread and worker threads") {
    asio::io_context ioc{1};
    asio::thread_pool pool(4);

    asio::co_spawn(
            ioc,
            [&pool]() -> asio::awaitable<void> { // NOLINT
                auto main_ext = asio::bind_executor(co_await asio::this_coro::executor);
                auto wrk_ext = asio::bind_executor(pool.get_executor());

                int cnt = 0;
                fmt::println("in thread/{} -> {}", std::this_thread::get_id(), cnt);

                co_await asio::post(wrk_ext);
                ++cnt;
                fmt::println("in thread/{} -> {}", std::this_thread::get_id(), cnt);

                co_await asio::post(main_ext);
                ++cnt;
                fmt::println("in thread/{} -> {}", std::this_thread::get_id(), cnt);

                co_await asio::post(wrk_ext);
                ++cnt;
                fmt::println("in thread/{} -> {}", std::this_thread::get_id(), cnt);

                co_return;
            },
            asio::detached);

    ioc.run();
    pool.join();
}

TEST_CASE("Post task after join completed") {
    asio::thread_pool pool(4);
    asio::post(pool.get_executor(), [] {
        fmt::println("task before join on thread={}", std::this_thread::get_id());
        std::this_thread::sleep_for(std::chrono::seconds(10));
        fmt::println("complete sleep on thread={}", std::this_thread::get_id());
    });
    std::jthread th([&pool] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        asio::post(pool.get_executor(), [] {
            fmt::println("new task-1 on thread={}", std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::seconds(5));
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));
        asio::post(pool.get_executor(), [] {
            fmt::println("new task-2 on thread={}", std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::seconds(5));
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));
        asio::post(pool.get_executor(), [] {
            fmt::println("new task-3 on thread={}", std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::seconds(5));
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));
        asio::post(pool.get_executor(), [] {
            fmt::println("new task-4 on thread={}", std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::seconds(5));
        });
    });

    fmt::println("start to join thread_pool");
    pool.join();
    fmt::println("join completed");

    asio::post(pool.get_executor(), [] {
        fmt::println("task after join");
    });
}
