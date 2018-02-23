/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include <utility>

#include <arpa/inet.h>
#include <netinet/in.h>

TcpConnection::TcpConnection(ScopedFD conn_fd, const struct sockaddr_in& addr)
    : conn_fd_(std::move(conn_fd)), port_(static_cast<int>(ntohs(addr.sin_port)))
{
    inet_ntop(AF_INET, &addr.sin_addr, ip_, kIPAddrLen);
    printf("Connected to client %s:%d\n", ip_, port_);
}
