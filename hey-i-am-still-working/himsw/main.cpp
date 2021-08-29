
#include <chrono>
#include <iostream>
#include <thread>

#include "himsw/labor_monitor.h"

int main() {
    while (true) {
        himsw::labor_monitor::instance().tick();
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }

    return 0;
}
