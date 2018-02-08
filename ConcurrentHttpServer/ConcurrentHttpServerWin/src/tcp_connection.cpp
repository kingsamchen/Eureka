/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include <WS2tcpip.h>

#include "kbase/basic_types.h"
#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

TcpConnection::TcpConnection()
    : state_(State::WaitConnect), client_port_(0)
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

    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    int len = sizeof(client_addr);
    getpeername(conn_socket_.get(), reinterpret_cast<sockaddr*>(&client_addr), &len);

    inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_ip_, kClientIPSize);
    client_port_ = ntohs(client_addr.sin_port);

    printf("Connects to %s:%hu on worker %u\n", client_ip_, client_port_, GetCurrentThreadId());

    state_ = State::WaitRequest;
}

void TcpConnection::ReadRequest()
{
    state_ = State::WaitRequest;

    buf_info_.buf = io_buf_;
    buf_info_.len = kIOBufSize;
    DWORD flags = 0;

    auto rv = WSARecv(conn_socket_.get(), &buf_info_, 1, nullptr, &flags, this, nullptr);
    if (rv == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
        LOG(WARNING) << "WSARecv() failed: " << WSAGetLastError();
        // TODO: reset.
    }
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
