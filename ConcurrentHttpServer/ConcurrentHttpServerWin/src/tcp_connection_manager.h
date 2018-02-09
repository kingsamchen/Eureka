/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TCP_CONNECTION_MANAGER_H_
#define TCP_CONNECTION_MANAGER_H_

#include <memory>
#include <mutex>
#include <vector>

#include <Windows.h>
#include <WinSock2.h>

#include "kbase/basic_macros.h"
#include "kbase/basic_types.h"
#include "kbase/scoped_handle.h"
#include "kbase/singleton.h"

#include "scoped_socket.h"

class TcpConnection;

class TcpConnectionManager {
public:
    static constexpr int kMaxListeners = 10;

    static TcpConnectionManager* GetInstance();

    DISALLOW_COPY(TcpConnectionManager);

    DISALLOW_MOVE(TcpConnectionManager);

    void Configure(unsigned short port, DWORD concurrent_workers);

    void ListenForClient();

    TcpConnection* AcceptNewClient();

    HANDLE io_port() const noexcept
    {
        return io_port_.get();
    }

    SOCKET listener() const noexcept
    {
        return listener_.get();
    }

private:
    TcpConnectionManager();

    friend struct kbase::DefaultSingletonTraits<TcpConnectionManager>;

private:
    static constexpr int kMaxPendingClients = 10;
    static constexpr DWORD kAcceptAddrLength = sizeof(sockaddr_in) + 16;

    bool configured_;

    kbase::ScopedWinHandle io_port_;
    ScopedSocketHandle listener_;
    ScopedSocketHandle accept_conn_;
    kbase::byte accept_addr_block_[kAcceptAddrLength * 2];
    OVERLAPPED accept_overlap_;

    // TODO: Improve look-up or management performance
    std::mutex conn_mgr_mutex_;
    std::vector<std::unique_ptr<TcpConnection>> free_;
    std::vector<std::unique_ptr<TcpConnection>> in_use_;
};

#endif  // TCP_CONNECTION_MANAGER_H_
