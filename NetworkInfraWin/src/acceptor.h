/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <functional>

#include "kbase/basic_macros.h"

#include "io_context.h"
#include "notifier.h"
#include "socket_address.h"
#include "scoped_socket.h"

namespace network {

class EventLoop;

class Acceptor {
public:
    using EventHandler = std::function<void(ScopedSocket&&, const SocketAddress&)>;

    Acceptor(EventLoop* loop, const SocketAddress& addr);

    ~Acceptor() = default;

    DISALLOW_COPY(Acceptor);

    DISALLOW_MOVE(Acceptor);

    void Listen();

    bool listenning() const noexcept
    {
        return listenning_;
    }

    const SocketAddress& listen_address() const noexcept
    {
        return server_addr_;
    }

    void SetNewConnectionHandler(const EventHandler& handler)
    {
        new_connection_handler_ = handler;
    }

private:
    void PostAccept();

    void HandleAcceptCompletion(IOContext* io_ctx);

private:
    static constexpr DWORD kAcceptAddrLength = sizeof(sockaddr_in) + 16;

    EventLoop* loop_;
    SocketAddress server_addr_;
    bool listenning_;

    ScopedSocket listener_;
    Notifier listener_notifier_;

    // We always keep accept_conn_ available for use.
    ScopedSocket accept_conn_;
    char accept_addr_block_[kAcceptAddrLength * 2];
    IOContext accept_ctx_;

    EventHandler new_connection_handler_;
};

}   // namespace network

#endif  // ACCEPTOR_H_
