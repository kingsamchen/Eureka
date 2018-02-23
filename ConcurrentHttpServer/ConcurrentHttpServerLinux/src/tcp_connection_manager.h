/*
 @ 0xCCCCCCCC
*/

#ifndef TCP_CONNECTION_MANAGER_H_
#define TCP_CONNECTION_MANAGER_H_

#include <memory>
#include <mutex>
#include <vector>

#include "kbase/basic_macros.h"
#include "kbase/singleton.h"

#include "scoped_fd.h"

class TcpConnection;

class TcpConnectionManager {
public:
    static TcpConnectionManager* GetInstance();

    DISALLOW_COPY(TcpConnectionManager);

    DISALLOW_MOVE(TcpConnectionManager);

    void Configure(unsigned short port);

    void AcceptNewClient();

    int epfd() const noexcept
    {
        return epfd_.get();
    }

private:
    TcpConnectionManager();

    int listen_fd() const noexcept
    {
        return listener_.get();
    }

    friend struct kbase::DefaultSingletonTraits<TcpConnectionManager>;

private:
    static constexpr size_t kMaxPendingClients = 10;

    bool configured_;

    ScopedFD epfd_;
    ScopedFD listener_;

    std::mutex conn_mutex_;
    std::vector<std::unique_ptr<TcpConnection>> connections_;
};

#endif  // TCP_CONNECTION_MANAGER_H_
