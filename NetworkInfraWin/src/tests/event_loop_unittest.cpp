/*
 @ 0xCCCCCCCC
*/

#include "gtest/gtest.h"

#include "../event_loop.h"

#include <thread>

namespace network {

TEST(EventLoopTest, LoopAndQuit)
{
    EventLoop loop;

    std::thread th([&loop] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        loop.Quit();
    });

    loop.Run();

    th.join();
}

}   // namespace network
