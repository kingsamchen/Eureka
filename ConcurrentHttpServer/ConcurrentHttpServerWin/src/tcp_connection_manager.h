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
#include <MSWSock.h>

#include "kbase/basic_macros.h"
#include "kbase/basic_types.h"
#include "kbase/scoped_handle.h"
#include "kbase/singleton.h"

#include "scoped_socket.h"

class TcpConnection;

namespace internal {

template<class T>
struct pointer_comp {
    typedef std::true_type is_transparent;

    struct helper {
        T* ptr;

        helper()
            : ptr(nullptr)
        {}

        helper(helper const&) = default;

        helper(T* p)
            : ptr(p)
        {}

        template<class U>
        helper(std::shared_ptr<U> const& sp)
            : ptr(sp.get())
        {}

        template<class U, class...Ts>
        helper(std::unique_ptr<U, Ts...> const& up)
            : ptr(up.get())
        {}

        bool operator<(helper o) const
        {
            return std::less<T*>()(ptr, o.ptr);
        }
    };

    bool operator()(helper const&& lhs, helper const&& rhs) const
    {
        return lhs < rhs;
    }
};

}   // namespace internal

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

    LPFN_ACCEPTEX pfn_acceptex_;

    // TODO: Improve look-up or management performance
    std::mutex conn_mgr_mutex_;
    std::vector<std::unique_ptr<TcpConnection>> free_;
    std::vector<std::unique_ptr<TcpConnection>> in_use_;
};

#endif  // TCP_CONNECTION_MANAGER_H_
