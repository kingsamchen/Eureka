/*
 @ 0xCCCCCCCC
*/

#include "acceptor.h"

#include "kbase/error_exception_util.h"

#include "event_loop.h"

namespace {

network::ScopedFD CreateNonBlockingSocketFD()
{
    network::ScopedFD sock_fd(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0));
    ENSURE(CHECK, static_cast<bool>(sock_fd)).Require();
    return sock_fd;
}

}   // namespace

namespace network {

Acceptor::Acceptor(EventLoop* loop, const SocketAddress& addr)
    : loop_(loop),
      server_addr_(addr),
      listenning_(false),
      listener_(CreateNonBlockingSocketFD()),
      listener_channel_(loop, listener_.get())
{
    // Enable reuse of address.
    int optval = 1;
    int rv = setsockopt(listener_.get(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    ENSURE(CHECK, rv == 0)(errno).Require();

    const auto& sock_addr = server_addr_.raw();
    rv = bind(listener_.get(), reinterpret_cast<const sockaddr*>(&sock_addr), sizeof(sock_addr));
    ENSURE(CHECK, rv == 0)(errno).Require();

    listener_channel_.set_read_handler([this] { HandleNewConnection(); });
}

void Acceptor::Listen()
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();

    listenning_ = true;

    int rv = listen(listener_.get(), SOMAXCONN);
    ENSURE(CHECK, rv == 0)(errno).Require();

    listener_channel_.EnableReading();
}

void Acceptor::HandleNewConnection()
{
    sockaddr_in addr {};
    socklen_t len = sizeof(addr);
    ScopedFD conn(accept4(listener_.get(),
                          reinterpret_cast<sockaddr*>(&addr),
                          &len,
                          SOCK_NONBLOCK | SOCK_CLOEXEC));
    ENSURE(CHECK, static_cast<bool>(conn))(errno).Require();

    SocketAddress peer_addr(addr);

    if (conn) {
        if (new_connection_handler_) {
            new_connection_handler_(std::move(conn), peer_addr);
        } else {
            conn = nullptr;
        }
    }
}

void Acceptor::set_new_connection_handler(const EventHandler& handler)
{
    new_connection_handler_ = handler;
}

}   // namespace network
