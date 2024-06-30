//
// 0xCCCCCCCC
//

#include <thread>
#include <vector>

#include "fmt/core.h"
#include "fmt/std.h"

// #include "coro_task/future.h"
// #include "coro_task/job.h"
#include "coro_task/job2.h"

// coro::future<int> create_future() {
//     co_return 42;
// }

// coro::job prepare_job() {
//     fmt::println("{}: {}", __PRETTY_FUNCTION__, std::this_thread::get_id());
//     co_await coro::my_suspend_never{};
// }

int main() {
    std::vector<std::jthread> thds(8);
    for (auto& thd : thds) {
        coro::perform_job2(thd);
    }
    return 0;
}
