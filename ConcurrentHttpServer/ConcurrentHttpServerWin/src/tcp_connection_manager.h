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
#include <set>
#include <vector>

#include <Windows.h>
#include <winsock2.h>

#include "kbase/basic_macros.h"
#include "kbase/singleton.h"

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

    TcpConnection* ListenForNewClient(SOCKET listener, HANDLE io_port);

    bool TryReclaim(TcpConnection* conn);

private:
    TcpConnectionManager();

    friend struct kbase::DefaultSingletonTraits<TcpConnectionManager>;

private:
    std::mutex conn_mutex_;

    std::vector<std::shared_ptr<TcpConnection>> free_;

    // Used connections consist of listening connections and connected connections.
    std::set<std::shared_ptr<TcpConnection>, internal::pointer_comp<TcpConnection>> used_;

    int listening_count_;
};

#endif  // TCP_CONNECTION_MANAGER_H_
