/*
 @ 0xCCCCCCCC
*/

#include "tcp_server.h"

#include "kbase/logging.h"
#include "kbase/string_format.h"

namespace network {

using namespace std::placeholders;

TCPServer::TCPServer(EventLoop* loop, const SocketAddress& listen_addr, const std::string& name)
    : loop_(loop),
      name_(name),
      acceptor_(loop, listen_addr),
      next_conn_id_(0)
{
    acceptor_.SetNewConnectionHandler(std::bind(&TCPServer::HandleNewConnection, this, _1, _2));
}

void TCPServer::Start()
{
    // FIXME: Run real start work on IO thread.
    if (!acceptor_.listenning()) {
        acceptor_.Listen();
    }
}

void TCPServer::HandleNewConnection(ScopedSocket&& conn_sock, const SocketAddress& peer_addr)
{
    // create a new tcp-connection.
    auto conn_name = kbase::StringFormat("{0}-{1}", name_, next_conn_id_++);

    LOG(INFO) << "New connection " << conn_name << " on " << peer_addr.ToHostPort();

    auto conn = std::make_shared<TCPConnection>(loop_, conn_name, std::move(conn_sock),
                                                acceptor_.listen_address(), peer_addr);

    connections_.insert({conn_name, conn});

    conn->SetConnectionHandler(connection_handler_);
    conn->SetMessageHandler(message_handler_);
    conn->SetCloseHandler([this](const TCPConnectionPtr& conn) { RemoveConnection(conn); });

    conn->EstablishConnection();
}

void TCPServer::RemoveConnection(const TCPConnectionPtr& conn)
{
    auto count = connections_.erase(conn->name());
    ENSURE(CHECK, count == 1)(count).Require();

    loop_->QueueTask(std::bind(&TCPConnection::DestroyConnection, conn));
}

void TCPServer::SetConnectionHandler(const ConnectionHandler& handler)
{
    connection_handler_ = handler;
}

void TCPServer::SetMessageHandler(const MessageHandler& handler)
{
    message_handler_ = handler;
}

}   // namespace network
