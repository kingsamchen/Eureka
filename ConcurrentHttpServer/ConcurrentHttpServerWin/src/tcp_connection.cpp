/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include <WS2tcpip.h>

#include "kbase/basic_types.h"
#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

#include "winsock_ctx.h"

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

    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    int len = sizeof(client_addr);
    getpeername(conn_socket_.get(), reinterpret_cast<sockaddr*>(&client_addr), &len);

    inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_ip_, kClientIPSize);

    printf("Connects to %s on worker %u\n", client_ip_, GetCurrentThreadId());

    ReadRequest();
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

void TcpConnection::Disconnect()
{
    state_ = State::WaitReset;
    winsock_ctx::DisconnectEx(conn_socket_.get(), this, 0, 0);
}

void TcpConnection::OnIOComplete(int64_t bytes_transferred)
{
    switch (state_) {
        case State::WaitRequest:
            OnReadRequestComplete(bytes_transferred);
            break;

        case State::WaitReset:
            OnDisconnectComplete();
            break;

        default:
            ENSURE(CHECK, kbase::NotReached())(kbase::enum_cast(state_)).Require();
            break;
    }
}

void TcpConnection::OnReadRequestComplete(int64_t bytes_transferred)
{
    if (bytes_transferred == 0) {
        Disconnect();
        return;
    }

    // TODO: implement request parse.

    std::string msg(io_buf_, bytes_transferred);

    printf("-> %s\n", msg.c_str());

    ReadRequest();
}

void TcpConnection::OnDisconnectComplete()
{
    printf("Client %s disconnected on worker %u!\n", client_ip_, GetCurrentThreadId());

    conn_socket_.reset();

    state_ = State::WaitConnect;
}
