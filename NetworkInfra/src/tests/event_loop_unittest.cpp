/*
 @ 0xCCCCCCCC
*/

#include <gtest/gtest.h>

#include <sys/timerfd.h>

#include "channel.h"
#include "event_loop.h"

namespace network {

TEST(EventLoopTest, Demo)
{
    EventLoop event_loop;

    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    Channel channel(&event_loop, timerfd);
    channel.set_read_handler([] { printf("Timeout!\n"); });
    channel.EnableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 2;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    event_loop.Run();

    close(timerfd);
}

}   // namespace network
