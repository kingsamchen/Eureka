/*
 @ 0xCCCCCCCC
*/

#include <gtest/gtest.h>

#include "event_loop.h"

namespace network {

TEST(EventLoopTest, Demo)
{
    EventLoop event_loop;
    event_loop.Run();
}

}   // namespace network
