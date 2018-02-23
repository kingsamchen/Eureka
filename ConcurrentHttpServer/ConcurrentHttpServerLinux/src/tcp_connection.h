/*
 @ 0xCCCCCCCC
*/

#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include "kbase/basic_macros.h"

#include "scoped_fd.h"

class TcpConnection {
public:
    TcpConnection(ScopedFD conn_fd, const struct sockaddr_in& addr);

    DISALLOW_COPY(TcpConnection);

    DISALLOW_MOVE(TcpConnection);

private:
    static constexpr socklen_t kIPAddrLen = 16;
    ScopedFD conn_fd_;
    int port_;
    char ip_[kIPAddrLen];
};

#endif  // TCP_CONNECTION_H_
