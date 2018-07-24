/*
 @ 0xCCCCCCCC
*/

#include "gtest/gtest.h"

#include "tcp_server.h"

namespace {

void OnConnection(const network::TCPConnectionPtr& conn)
{
    if (conn->connected()) {
        printf("OnConnection(): new connection %s from %s\n",
               conn->name().c_str(), conn->peer_address().ToHostPort().c_str());
    } else {
        printf("OnConnection(): disclose connection %s %s\n",
               conn->name().c_str(), conn->peer_address().ToHostPort().c_str());
    }
}

void OnMessage(const network::TCPConnectionPtr& conn, const void* data, size_t len)
{
    printf("onMessage(): received %zd bytes from connection [%s]\ndata: %s\n",
           len, conn->name().c_str(), static_cast<const char*>(data));
}

}   // namespace

namespace network {

TEST(TCPServerTest, General)
{
    EventLoop loop;

    SocketAddress listen_addr(9876);

    TCPServer server(&loop, listen_addr, "KServer");

    server.SetConnectionHandler(&OnConnection);
    server.SetMessageHandler(&OnMessage);

    server.Start();

    loop.Run();
}

}   // namespace network
