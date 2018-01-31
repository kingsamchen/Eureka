/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection_manager.h"

#include "tcp_connection.h"

// static
TcpConnectionManager* TcpConnectionManager::GetInstance()
{
    return kbase::Singleton<TcpConnectionManager>::instance();
}

TcpConnectionManager::TcpConnectionManager()
    : listening_count_(0)
{}

TcpConnection* TcpConnectionManager::ListenForNewClient(SOCKET listener, HANDLE io_port)
{
    std::shared_ptr<TcpConnection> conn;

    {
        std::lock_guard<std::mutex> lock(conn_mutex_);

        if (!free_.empty()) {
            conn = std::move(free_.back());
            free_.pop_back();
        } else {
            conn = std::make_shared<TcpConnection>(listener, io_port);
        }

        used_.insert(conn);
        conn->WaitForAccept();
        ++listening_count_;
    }

    return conn.get();
}

bool TcpConnectionManager::TryReclaim(TcpConnection* conn)
{
    std::lock_guard<std::mutex> lock(conn_mutex_);

    if (listening_count_ < kMaxListeners) {
        return false;
    }

    auto it = used_.find(conn);
    free_.push_back(*it);
    used_.erase(it);
    return true;
}
