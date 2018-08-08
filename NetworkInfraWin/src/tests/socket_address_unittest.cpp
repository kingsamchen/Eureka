/*
 @ 0xCCCCCCCC
*/

#include "gtest/gtest.h"

#include "../socket_address.h"

namespace network {

TEST(SocketAddressTest, FromPort)
{
    SocketAddress addr(1234);
    EXPECT_EQ(1234, addr.port());
    EXPECT_EQ("0.0.0.0:1234", addr.ToHostPort());
}

TEST(SocketAddressTest, FromHostPort)
{
    SocketAddress addr("127.0.0.1", 8888);
    EXPECT_EQ(8888, addr.port());
    EXPECT_EQ("127.0.0.1:8888", addr.ToHostPort());
}

}   // namespace network
