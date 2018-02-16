/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <cstdint>
#include <string>

#include <Windows.h>

#include "kbase/basic_macros.h"
#include "kbase/path.h"

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

    void OnIOComplete(int64_t bytes_transferred);

private:
    void ReadRequest();

    void WriteResponse();

    void TransmitData();

    void Disconnect();

    void OnReadComplete(int64_t bytes_read);

    void OnWriteComplete(int64_t bytes_written);

    void OnDisconnectComplete();

private:
    static constexpr size_t kIOBufSize = 1024;
    static constexpr size_t kClientIPSize = 16;

    State state_;
    ScopedSocketHandle conn_socket_;
    char client_ip_[kClientIPSize];

    kbase::Path data_dir_;
    kbase::ScopedWinHandle file_;

    WSABUF buf_info_;
    char io_buf_[kIOBufSize];

    // Specialized for HTTP transactions.
    std::string request_;

    size_t sent_size_;
    size_t left_sent_size_;
    size_t remaining_data_size_;
};

#endif  // TCP_CONNECTION_H_
