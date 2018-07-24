/*
 @ 0xCCCCCCCC
*/

#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <memory>
#include <string>

#include "kbase/basic_macros.h"
#include "kbase/error_exception_util.h"

#include "channel.h"
#include "event_handlers.h"
#include "event_loop.h"
#include "scoped_fd.h"
#include "socket_address.h"

namespace network {

class EventLoop;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
private:
    enum class State {
        Connecting,
        Connected,
        Disconnted
    };

public:
    TCPConnection(EventLoop* loop, const std::string& name, ScopedFD&& sockfd,
                  const SocketAddress& local_addr, const SocketAddress& peer_addr);

    ~TCPConnection();

    DISALLOW_COPY(TCPConnection);

    DISALLOW_MOVE(TCPConnection);

    void EstablishConnection();

    void DestroyConnection();

    const std::string& name() const noexcept
    {
        return name_;
    }

    const SocketAddress& peer_address() const noexcept
    {
        return peer_addr_;
    }

    bool connected() const noexcept
    {
        return state_ == State::Connected;
    }

    void SetConnectionHandler(const ConnectionHandler& handler)
    {
        connection_handler_ = handler;
    }

    void SetMessageHandler(const MessageHandler& handler)
    {
        message_handler_ = handler;
    }

    void SetCloseHandler(const CloseHandler& handler)
    {
        close_handler_ = handler;
    }

private:
    void HandleRead();

    void HandleClose();

    void HandleError();

    void set_state(State new_state) noexcept
    {
        ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();
        state_ = new_state;
    }

private:
    EventLoop* loop_;
    std::string name_;
    ScopedFD sockfd_;
    Channel conn_channel_;
    State state_;
    SocketAddress local_addr_;
    SocketAddress peer_addr_;

    ConnectionHandler connection_handler_;
    MessageHandler message_handler_;
    CloseHandler close_handler_;
};

using TCPConnectionPtr = std::shared_ptr<TCPConnection>;

}   // namespace network

#endif  // TCP_CONNECTION_H_
