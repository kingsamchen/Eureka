/*
 @ 0xCCCCCCCC
*/

#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <string>

#include "kbase/basic_macros.h"
#include "kbase/path.h"

#include "scoped_fd.h"

class TcpConnection {
public:
    TcpConnection(ScopedFD conn_fd, const struct sockaddr_in& addr);

    DISALLOW_COPY(TcpConnection);

    DISALLOW_MOVE(TcpConnection);

    void Read();

    void Write();

private:
    void PrepareResponse();

private:
    static constexpr socklen_t kIPAddrLen = 16;
    static constexpr size_t kIOBufSize = 1024;

    ScopedFD conn_fd_;
    int port_;
    char ip_[kIPAddrLen];

    kbase::Path data_dir_;

    char io_buf_[kIOBufSize];

    std::string request_;
    std::string response_;
    size_t response_sent_;
};

#endif  // TCP_CONNECTION_H_
