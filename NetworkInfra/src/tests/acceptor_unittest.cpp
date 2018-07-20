/*
 @ 0xCCCCCCCC
*/

#include <cstdio>

#include "gtest/gtest.h"

#include "acceptor.h"
#include "event_loop.h"

namespace network {

TEST(AcceptorTest, General)
{
    SocketAddress server_addr(9876);

    EventLoop loop;

    Acceptor acceptor(&loop, server_addr);
    acceptor.set_new_connection_handler([](auto& conn_fd, const auto& peer_addr) {
        printf("Accepted a new connection from %s\n", peer_addr.ToHostPort().c_str());
        std::string msg("How are you?\n");
        write(conn_fd.get(), msg.data(), msg.size());
        conn_fd = nullptr;
    });

    acceptor.Listen();

    loop.Run();
}

}   // namespace network
