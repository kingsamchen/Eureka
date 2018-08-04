/*
 @ 0xCCCCCCCC
*/

#include "gtest/gtest.h"

#include "../winsock_context.h"

#include "kbase/basic_macros.h"

namespace network {

TEST(WinsockContextTest, General)
{
    WinsockContext ctx;
    UNUSED_VAR(ctx);

    EXPECT_TRUE(WinsockContext::instance().AcceptEx != nullptr);
    EXPECT_TRUE(WinsockContext::instance().DisconnectEx != nullptr);
}

TEST(WinsockContextTest, Lifetime)
{
    {
        WinsockContext ctx;
        UNUSED_VAR(ctx);
    }

    {
        WinsockContext actx;
        UNUSED_VAR(actx);
    }
}

}   // namespace network
