//
// Kingsley Chen
//

#include <iostream>

#include "coro_task/future.h"

coro::future<int> create_future() {
    co_return 42;
}

int main() {
    auto fut = create_future();
    std::cout << "fut.get(): " << fut.get();
    return 0;
}
