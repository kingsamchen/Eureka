//
// Kingsley Chen
//

#include <latch>
#include <thread>
#include <vector>

#include "fmt/format.h"
#include "fmt/ranges.h"

int main() {
    // 4 slots.
    std::vector<int> v(4);

    const auto cnt = v.size();
    std::latch latch{static_cast<int>(cnt)};

    for (std::size_t i = 0; i < cnt; ++i) {
        std::jthread th([&latch, &v, i] {
            fmt::println("\tprocess on the worker-{}", i);
            std::this_thread::sleep_for(std::chrono::seconds{i});
            v[i] = static_cast<int>(i) + 1;
            fmt::println("\tdone on the worker-{}", i);
            latch.arrive_and_wait();
            fmt::println("\tworker-{} quit", i);
        });
        th.detach();
    }

    fmt::println("main thread wait on workers");
    latch.wait();

    fmt::println("result={}", v);

    return 0;
}
