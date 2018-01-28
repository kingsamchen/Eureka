/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include "kbase/error_exception_util.h"

#include "iocp_utils.h"

TcpConnection::TcpConnection(SOCKET listener, HANDLE io_port)
    : listener_(listener)
{
    conn_socket_.reset(WSASocketW(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED));
    ENSURE(THROW, !!conn_socket_)(WSAGetLastError()).Require();

    utils::AssociateDeviceWithIOCP(reinterpret_cast<HANDLE>(conn_socket_.get()), io_port,
                                   utils::CompletionKeyIO);
}

void TcpConnection::WaitForAccept()
{
    // TODO:
}

void TcpConnection::OnIOComplete(int64_t bytes_transferred)
{
    // TODO:
}
