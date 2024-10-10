//
// Kingsley Chen
//

#include <chrono>
#include <thread>

#include "fmt/format.h"
#include "fmt/std.h"

int main() {
    std::jthread th([](std::stop_token stop_token) {
        while (!stop_token.stop_requested()) {
            fmt::println("\tsimulate processing on worker thread [{}]...",
                         std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::seconds{1});
        }

        fmt::println("\tworker thread exit...");
    });

    // set up stop_callback
    // will be invoked on the thread that requests the stop
    std::stop_callback scb(th.get_stop_token(), [] {
        fmt::println("\tstop was requested on thread [{}]", std::this_thread::get_id());
    });

    fmt::println("main thread [{}] is processing sth...", std::this_thread::get_id());
    std::this_thread::sleep_for(std::chrono::seconds{3});
    fmt::println("requesting stop worker thread");
    (void)th.request_stop();
    fmt::println("waiting for worker thread to quit");
    th.join();

    return 0;
}
