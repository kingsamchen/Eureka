/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TCP_CONNECTION_MANAGER_H_
#define TCP_CONNECTION_MANAGER_H_

#include <atomic>
#include <memory>
#include <vector>

#include "kbase/basic_macros.h"
#include "kbase/singleton.h"

class TcpConnection;

class TcpConnectionManager {
public:
    static TcpConnectionManager* GetInstance();

    DISALLOW_COPY(TcpConnectionManager);

    DISALLOW_MOVE(TcpConnectionManager);

    TcpConnection* ListenForClient();

    void Reclaim(TcpConnection* conn);

private:
    TcpConnectionManager();

    friend struct kbase::DefaultSingletonTraits<TcpConnectionManager>;

private:
    std::vector<std::unique_ptr<TcpConnection>> free_;

    // Used connections consist of listening connections and connected connections.
    std::vector<std::unique_ptr<TcpConnection>> used_;

    std::atomic<int> listening_count_;
};

#endif  // TCP_CONNECTION_MANAGER_H_
