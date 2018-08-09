/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <memory>
#include <string>

#include "kbase/basic_macros.h"
#include "kbase/error_exception_util.h"

#include "buffer.h"
#include "common_event_handlers.h"
#include "event_loop.h"
#include "notifier.h"
#include "scoped_socket.h"
#include "socket_address.h"

namespace network {

class EventLoop;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
    enum class State {
        Connecting,
        Connected,
        Disconnecting,
        Disconnected
    };

    TCPConnection(EventLoop* loop, const std::string& name, ScopedSocket&& sock,
                  const SocketAddress& local_addr, const SocketAddress& peer_addr);

    ~TCPConnection();

    DISALLOW_COPY(TCPConnection);

    DISALLOW_MOVE(TCPConnection);

    void EstablishConnection();

    void DestroyConnection();

    // Should be thread-safe.
    void Send(const std::string& msg);

    void Shutdown();

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

private:
    void set_state(State new_state) noexcept
    {
        ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();
        state_ = new_state;
    }

    void SendInLoop(const std::string& msg);

    void ShutdownInLoop();

    void PostReadProbe();

    void PostRead();

    void PostWrite();

    void HandleReadCompletion(IOContext* io_ctx);

    void HandleWriteCompletion(IOContext* io_ctx);

    void HandleClose(IOContext* io_ctx);

private:
    EventLoop* loop_;
    std::string name_;
    ScopedSocket socket_;
    Notifier sock_notifier_;
    State state_;
    SocketAddress local_addr_;
    SocketAddress peer_addr_;

    ConnectionHandler connection_handler_;
    MessageHandler message_handler_;
    CloseHandler close_handler_;

    Buffer input_buf_;
    Buffer output_buf_;

    IOContext read_ctx_;
    IOContext write_ctx_;

    int oustanding_writes_;
};

}   // namespace network

#endif  // TCP_CONNECTION_H_
