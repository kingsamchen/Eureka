//
// 0xCCCCCCCC
//

#include <thread>

#include "fmt/core.h"
#include "fmt/std.h"

// #include "coro_task/future.h"
#include "coro_task/job.h"

// coro::future<int> create_future() {
//     co_return 42;
// }

coro::job prepare_job() {
    fmt::println("{}: {}", __PRETTY_FUNCTION__, std::this_thread::get_id());
    co_await coro::my_suspend_never{};
}

int main() {
    fmt::println("{}: {}", __PRETTY_FUNCTION__, std::this_thread::get_id());
    auto job = prepare_job();
    fmt::println("start the job");
    job.start();
    fmt::println("after the job");
    return 0;
}
