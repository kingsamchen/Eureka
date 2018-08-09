/*
 @ 0xCCCCCCCC
*/

#include "gtest/gtest.h"

#include <string>

#include "tcp_connection.h"
#include "tcp_server.h"
#include "winsock_context.h"

namespace {

std::string message;

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

TEST(TCPServerTest, ReadWrite)
{
    WinsockContext winsock_ctx;

    EventLoop loop;

    SocketAddress listen_addr(9876);

    TCPServer server(&loop, listen_addr, "KServer");

    server.SetConnectionHandler(&OnConnectionEx);
    server.SetMessageHandler(&OnMessage);

    message = "Tic tac toe";

    server.Start();

    loop.Run();
}

TEST(TCPServerTest, Echo)
{
    WinsockContext winsock_ctx;

    EventLoop loop;

    SocketAddress listen_addr(9876);

    TCPServer server(&loop, listen_addr, "KServer");

    server.SetConnectionHandler([](const network::TCPConnectionPtr& conn) {
        if (conn->connected()) {
            printf("onConnection(): new connection [%s] from %s\n",
               conn->name().c_str(),
               conn->peer_address().ToHostPort().c_str());
        } else {
            printf("OnConnection(): disclose connection %s %s\n",
               conn->name().c_str(), conn->peer_address().ToHostPort().c_str());
        }
    });

    server.SetMessageHandler([](const network::TCPConnectionPtr& conn, network::Buffer& buf) {
        auto it = std::find(buf.begin(), buf.end(), '\n');
        if (it == buf.end()) {
            return;
        }

        auto len = std::distance(buf.begin(), it);
        auto msg = buf.ReadAsString(len);
        buf.ReadAsInt8();
        if (msg == "quit") {
            printf("Bye\n");
            conn->Shutdown();
        } else {
            printf("-> %s\n", msg.c_str());
            conn->Send(msg);
        }
    });

    server.Start();

    loop.Run();
}

}   // namespace network
