/*
 @ 0xCCCCCCCC
*/

#include <gtest/gtest.h>

#include <thread>

#include "event_loop.h"

#include <sys/syscall.h>

namespace network {

TEST(ReactorTest, SimpleTimer)
{
    EventLoop event_loop;

    event_loop.RunTaskAfter(chrono::seconds(5), [] {
        printf("Timer tick!\n");
    });

    event_loop.RunTaskAfter(chrono::seconds(2), [] {
        printf("Timer tick!\n");
    });

    event_loop.RunTaskAfter(chrono::seconds(7), [] {
        printf("Final Timer tick!\n");
        EventLoop::current()->Quit();
    });

    event_loop.Run();
}

TEST(ReactorTest, RunFromOtherThread)
{
    EventLoop loop;

    std::thread th([&loop] {
        loop.RunTask([] {
            printf("Run task on thread %ld\n", syscall(SYS_gettid));
        });

        loop.RunTaskAfter(chrono::seconds(3), [&loop] {
            printf("Delayed task\n");
            loop.Quit();
        });
    });

    loop.RunTask([] {
        printf("Run task on thread %ld\n", syscall(SYS_gettid));
    });

    loop.Run();

    th.join();
}

}   // namespace network
