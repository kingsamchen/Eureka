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

TcpConnection* TcpConnectionManager::ListenForClient()
{
    // TODO:
    return nullptr;
}

void TcpConnectionManager::Reclaim(TcpConnection* conn)
{
    // TODO:
}
