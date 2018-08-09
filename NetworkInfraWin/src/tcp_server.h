/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <functional>
#include <string>
#include <unordered_map>

#include "kbase/basic_macros.h"

#include "acceptor.h"
#include "scoped_socket.h"
#include "socket_address.h"
#include "tcp_connection.h"

namespace network {

class EventLoop;

class TCPServer {
public:
    TCPServer(EventLoop* loop, const SocketAddress& listen_addr, const std::string& name);

    ~TCPServer() = default;

    DISALLOW_COPY(TCPServer);

    DISALLOW_MOVE(TCPServer);

    // Should be thread-safe & immune for multiple calls.
    void Start();

    void SetConnectionHandler(const ConnectionHandler& handler);

    void SetMessageHandler(const MessageHandler& handler);

private:
    void HandleNewConnection(ScopedSocket&& conn_sock, const SocketAddress& peer_addr);

    void RemoveConnection(const TCPConnectionPtr& conn);

private:
    EventLoop* loop_;
    std::string name_;
    Acceptor acceptor_;
    int next_conn_id_;

    using ConnectionMap = std::unordered_map<std::string, TCPConnectionPtr>;

    ConnectionMap connections_;

    ConnectionHandler connection_handler_;
    MessageHandler message_handler_;
};

}   // namespace network

#endif  // TCP_SERVER_H_
