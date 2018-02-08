/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <cstdint>

#include <Windows.h>

#include "kbase/basic_macros.h"

#include "scoped_socket.h"

class TcpConnection : public OVERLAPPED {
    enum class State {
        WaitConnect,
        WaitRequest,
        WaitResponse,
        WaitReset
    };

public:
    TcpConnection();

    ~TcpConnection() = default;

    DISALLOW_COPY(TcpConnection);

    void Conncect(ScopedSocketHandle&& conn_socket);

    void ReadRequest();

    void WriteResponse();

    void OnIOComplete(int64_t bytes_transferred);

private:
    void OnReadRequestComplete(int64_t bytes_transferred);

private:
    static constexpr size_t kIOBufSize = 1024;
    static constexpr size_t kClientIPSize = 16;

    State state_;
    ScopedSocketHandle conn_socket_;
    char client_ip_[kClientIPSize];
    unsigned short client_port_;

    // TODO: buffer & request & response data
    WSABUF buf_info_;
    char io_buf_[kIOBufSize];
};

#endif  // TCP_CONNECTION_H_
