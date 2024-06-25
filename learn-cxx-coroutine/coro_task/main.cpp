//
// Kingsley Chen
//

#include <iostream>
#include <thread>

#include "fmt/core.h"
#include "fmt/std.h"

#include "coro_task/future.h"

coro::future<int> create_future() {
    co_return 42;
}

int main() {
    fmt::println("{}: {}", __PRETTY_FUNCTION__, std::this_thread::get_id());

    auto fut = create_future();
    std::cout << "fut.get(): " << fut.get();
    return 0;
}
