//
// Kingsley Chen
//

#include <chrono>
#include <semaphore>
#include <thread>

#include "fmt/format.h"

using namespace std::chrono_literals;

int main() {
    std::counting_semaphore<> sem{0};
    sem.release();
    std::jthread th([&sem] {
        fmt::println("enter worker thread");
        sem.acquire();
        fmt::println("worker thread acquired");
    });

    std::this_thread::sleep_for(1s);
    return 0;
}
