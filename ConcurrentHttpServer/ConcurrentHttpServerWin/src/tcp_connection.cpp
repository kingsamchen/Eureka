/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include "kbase/basic_types.h"
#include "kbase/error_exception_util.h"

TcpConnection::TcpConnection()
    : state_(State::WaitConnect)
{
    Internal = 0;
    InternalHigh = 0;
    Offset = 0;
    OffsetHigh = 0;
    hEvent = nullptr;
}

void TcpConnection::Conncect(ScopedSocketHandle&& conn_socket)
{
    ENSURE(CHECK, !conn_socket_).Require("Connection socket must be empty!");
    conn_socket_ = std::move(conn_socket);
    state_ = State::WaitRequest;
}

void TcpConnection::ReadRequest()
{
    state_ = State::WaitRequest;

    buf_info_.buf = io_buf_;
    buf_info_.len = kIOBufSize;
    DWORD flags = 0;

    auto r = WSARecv(conn_socket_.get(), &buf_info_, 1, nullptr, &flags, this, nullptr);
    ENSURE(CHECK, r != SOCKET_ERROR)(WSAGetLastError()).Require();
}

void TcpConnection::WriteResponse()
{
    state_ = State::WaitResponse;
}

void TcpConnection::OnIOComplete(int64_t bytes_transferred)
{
    switch (state_) {
        case State::WaitRequest:
            OnReadRequestComplete(bytes_transferred);
            break;

        default:
            ENSURE(CHECK, kbase::NotReached())(kbase::enum_cast(state_)).Require();
            break;
    }
}

void TcpConnection::OnReadRequestComplete(int64_t bytes_transferred)
{}
