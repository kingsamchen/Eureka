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
#include <winsock2.h>

#include "kbase/basic_macros.h"

#include "scoped_socket.h"

class TcpConnection : public OVERLAPPED {
public:
    TcpConnection(SOCKET listener, HANDLE io_port);

    ~TcpConnection() = default;

    DISALLOW_COPY(TcpConnection);

    void WaitForAccept();

    void OnIOComplete(int64_t bytes_transferred);

private:
    SOCKET listener_;
    ScopedSocketHandle conn_socket_;
};

#endif  // TCP_CONNECTION_H_
