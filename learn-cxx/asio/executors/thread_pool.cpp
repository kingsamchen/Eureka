//
// 0xCCCCCCCC
//

#include <latch>
#include <mutex>
#include <thread>
#include <unordered_set>

#include <asio/dispatch.hpp>
#include <asio/post.hpp>
#include <asio/thread_pool.hpp>
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
