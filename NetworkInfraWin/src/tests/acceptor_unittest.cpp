/*
 @ 0xCCCCCCCC
*/

#include "gtest/gtest.h"

#include "../acceptor.h"
#include "../event_loop.h"
#include "../winsock_context.h"

namespace network {

TEST(AcceptorTest, AcceptNew)
{
    WinsockContext winsockctx;

    SocketAddress srv_addr(9876);

    EventLoop loop;

    Acceptor acceptor(&loop, srv_addr);

    acceptor.SetNewConnectionHandler([](auto&& conn, const auto& peer_addr) {
        printf("Accepted a new connection from %s\n", peer_addr.ToHostPort().c_str());
        std::string msg("How are you?\n");
        send(conn.get(), msg.data(), static_cast<int>(msg.size()), 0);
        conn = nullptr;
    });

    acceptor.Listen();

    loop.Run();
}

}   // namespace network
