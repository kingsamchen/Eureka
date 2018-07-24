/*
 @ 0xCCCCCCCC
*/

#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <functional>

#include "kbase/basic_macros.h"

#include "channel.h"
#include "scoped_fd.h"
#include "socket_address.h"

namespace network {

class EventLoop;

class Acceptor {
public:
    // FIXME: conn_fd tpye.
    using EventHandler = std::function<void(ScopedFD&& conn_fd,
                                            const SocketAddress& peer_addr)>;

    Acceptor(EventLoop* loop, const SocketAddress& addr);

    ~Acceptor() = default;

    DISALLOW_COPY(Acceptor);

    DISALLOW_MOVE(Acceptor);

    void Listen();

    void set_new_connection_handler(const EventHandler& handler);

    bool listenning() const noexcept
    {
        return listenning_;
    }

    const SocketAddress& listen_address() const noexcept
    {
        return server_addr_;
    }

private:
    void HandleNewConnection();

private:
    EventLoop* loop_;
    SocketAddress server_addr_;
    bool listenning_;
    ScopedFD listener_;
    Channel listener_channel_;
    EventHandler new_connection_handler_;
};

}   // namespace network

#endif  // ACCEPTOR_H_
