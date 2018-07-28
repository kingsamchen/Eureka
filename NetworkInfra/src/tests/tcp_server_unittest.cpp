/*
 @ 0xCCCCCCCC
*/

#include "gtest/gtest.h"

#include "tcp_server.h"

namespace {

std::string message;

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

void OnConnectionEx(const network::TCPConnectionPtr& conn)
{
    if (conn->connected()) {
        printf("onConnection(): new connection [%s] from %s\n",
               conn->name().c_str(),
               conn->peer_address().ToHostPort().c_str());
        conn->Send(message);
        conn->Shutdown();
    } else {
        printf("OnConnection(): disclose connection %s %s\n",
               conn->name().c_str(), conn->peer_address().ToHostPort().c_str());
    }
}

void OnMessage(const network::TCPConnectionPtr& conn, network::Buffer& buf)
{
    printf("onMessage(): received %zd bytes from connection [%s]\ndata: %s\n",
           buf.readable_size(), conn->name().c_str(), buf.ReadAllAsString().c_str());
}

}   // namespace

namespace network {

TEST(TCPServerTest, Read)
{
    EventLoop loop;

    SocketAddress listen_addr(9876);

    TCPServer server(&loop, listen_addr, "KServer");

    server.SetConnectionHandler(&OnConnection);
    server.SetMessageHandler(&OnMessage);

    server.Start();

    loop.Run();
}

TEST(TCPServerTest, ReadWrite)
{
    EventLoop loop;

    SocketAddress listen_addr(9876);

    TCPServer server(&loop, listen_addr, "KServer");

    server.SetConnectionHandler(&OnConnectionEx);
    server.SetMessageHandler(&OnMessage);

    message = "Tic tac toe";

    server.Start();

    loop.Run();
}

}   // namespace network
