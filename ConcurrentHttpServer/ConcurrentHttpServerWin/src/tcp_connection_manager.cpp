/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection_manager.h"

#include "kbase/error_exception_util.h"

#include "iocp_utils.h"
#include "tcp_connection.h"
#include "winsock_ctx.h"

namespace {

ScopedSocketHandle CreateListener(unsigned short port, int max_pending_clients)
{
    ScopedSocketHandle listener(
        WSASocketW(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED));
    ENSURE(CHECK, !!listener).Require();

    sockaddr_in server_addr {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int rv = bind(listener.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    rv = listen(listener.get(), max_pending_clients);
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    return listener;
}

ScopedSocketHandle NewAcceptSocket()
{
    ScopedSocketHandle accept(
        WSASocketW(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED));
    ENSURE(CHECK, !!accept)(WSAGetLastError()).Require();

    return accept;
}

}   // namespace

// static
TcpConnectionManager* TcpConnectionManager::GetInstance()
{
    return kbase::Singleton<TcpConnectionManager>::instance();
}

TcpConnectionManager::TcpConnectionManager()
    : configured_(false)
{}

void TcpConnectionManager::Configure(unsigned short port, DWORD concurrent_workers)
{
    io_port_ = utils::CreateNewIOCP(concurrent_workers);
    ENSURE(CHECK, !!io_port_)(kbase::LastError()).Require();

    listener_ = CreateListener(port, kMaxPendingClients);

    bool success = utils::AssociateDeviceWithIOCP(reinterpret_cast<HANDLE>(listener_.get()),
                                                  io_port_.get(),
                                                  utils::CompletionKeyAccept);
    ENSURE(CHECK, success)(kbase::LastError()).Require();

    configured_ = true;
}

void TcpConnectionManager::ListenForClient()
{
    ENSURE(CHECK, !accept_conn_).Require();

    accept_conn_ = NewAcceptSocket();
    utils::AssociateDeviceWithIOCP(reinterpret_cast<HANDLE>(accept_conn_.get()),
                                   io_port(),
                                   utils::CompletionKeyIO);

    // Empty data for use.
    memset(accept_addr_block_, 0, kAcceptAddrLength);
    memset(&accept_overlap_, 0, sizeof(accept_overlap_));

    DWORD receive_len = 0;
    winsock_ctx::AcceptEx(listener(), accept_conn_.get(), accept_addr_block_, 0, kAcceptAddrLength,
                          kAcceptAddrLength, &receive_len, &accept_overlap_);
}

TcpConnection* TcpConnectionManager::AcceptNewClient()
{
    auto listen_socket = listener();
    setsockopt(accept_conn_.get(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
               reinterpret_cast<const char*>(&listen_socket), sizeof(SOCKET));

    TcpConnection* conn = nullptr;

    {
        std::lock_guard<std::mutex> lock(conn_mgr_mutex_);

        std::unique_ptr<TcpConnection> client_conn;

        if (free_.empty()) {
            client_conn = std::make_unique<TcpConnection>();
        } else {
            client_conn = std::move(free_.back());
            free_.pop_back();
        }

        conn = client_conn.get();

        in_use_.push_back(std::move(client_conn));
    }

    conn->Conncect(std::move(accept_conn_));

    return conn;
}

void TcpConnectionManager::Reclaim(TcpConnection* conn)
{
    {
        std::lock_guard<std::mutex> lock(conn_mgr_mutex_);

        auto it = std::find_if(in_use_.begin(), in_use_.end(), [conn](const auto& in_use_conn) {
                      return conn == in_use_conn.get();
                  });

        free_.push_back(std::move(*it));
        in_use_.erase(it);
    }

    printf("A connection is reclaimed to free list!\n");
}
