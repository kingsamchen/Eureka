/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include "kbase/basic_types.h"
#include "kbase/logging.h"

namespace {

int GetLastSocketError(int sockfd)
{
    int optval;
    socklen_t optlen = sizeof(optval);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen)) {
        // Failed...
        return errno;
    } else {
        return optval;
    }
}

}   // namespace

namespace network {

TCPConnection::TCPConnection(EventLoop* loop, const std::string& name, ScopedFD&& sockfd,
                             const SocketAddress& local_addr, const SocketAddress& peer_addr)
    : loop_(loop),
      name_(name),
      sockfd_(std::move(sockfd)),
      conn_channel_(loop, sockfd_.get()),
      state_(State::Connecting),
      local_addr_(local_addr),
      peer_addr_(peer_addr)
{
    conn_channel_.set_read_handler([this] { HandleRead(); });
    conn_channel_.set_write_handler([this] { HandleWrite(); });
    conn_channel_.set_close_handler([this] { HandleClose(); });
    conn_channel_.set_error_handler([this] { HandleError(); });
}

TCPConnection::~TCPConnection()
{
    LOG(INFO) << "Destruction of [" << name_ << "] at " << this << " with fd = " << sockfd_.get();
}

void TCPConnection::EstablishConnection()
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();
    ENSURE(CHECK, state_ == State::Connecting)(kbase::enum_cast(state_)).Require();

    set_state(State::Connected);

    conn_channel_.EnableReading();

    if (connection_handler_) {
        connection_handler_(shared_from_this());
    }
}

void TCPConnection::DestroyConnection()
{
    set_state(State::Disconnted);

    conn_channel_.DisableAll();

    connection_handler_(shared_from_this());

    loop_->RemoveChannel(&conn_channel_);
}

void TCPConnection::Send(const std::string& msg)
{
    if (state_ != State::Connected) {
        return;
    }

    // TODO: Optimize with move.
    loop_->RunTask([this, msg] {
        SendInLoop(msg);
    });
}

void TCPConnection::SendInLoop(const std::string& msg)
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();

    // TODO: Optimize with sending strategies.
    output_buf_.Write(msg.data(), msg.size());
    if (!conn_channel_.IsWritingEnabled()) {
        conn_channel_.EnableWriting();
    }
}

void TCPConnection::Shutdown()
{
    if (state_ == State::Connected) {
        state_ = State::Disconnecting;
        loop_->RunTask([this] { ShutdownInLoop(); });
    }
}

void TCPConnection::ShutdownInLoop()
{
    if (!conn_channel_.IsWritingEnabled()) {
        // TODO: handle failure.
        shutdown(sockfd_.get(), SHUT_WR);
    }
}

void TCPConnection::HandleRead()
{
    // TODO: Handle errno.
    ssize_t n = ReadFD(sockfd_.get(), input_buf_);
    if (n > 0) {
        if (message_handler_) {
            message_handler_(shared_from_this(), input_buf_);
        }
    } else if (n == 0) {
        HandleClose();
    } else {
        HandleError();
    }
}

void TCPConnection::HandleWrite()
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();

    if (!conn_channel_.IsWritingEnabled()) {
        LOG(INFO) << "Connection is down, no more writing!";
        return;
    }

    ssize_t n = write(sockfd_.get(), output_buf_.Peek(), output_buf_.readable_size());
    if (n <= 0) {
        LOG(WARNING) << "Trying to write to socket failed!";
        return;
    }

    output_buf_.Consume(static_cast<size_t>(n));
    if (output_buf_.readable_size() == 0) {
        conn_channel_.DisableWriting();
        if (state_ == State::Disconnecting) {
            ShutdownInLoop();
        }
    }
}

void TCPConnection::HandleClose()
{
    ENSURE(CHECK, state_ == State::Connected || state_ == State::Disconnecting)
        (kbase::enum_cast(state_)).Require();

    conn_channel_.DisableAll();

    close_handler_(shared_from_this());
}

void TCPConnection::HandleError()
{
    LOG(WARNING) << "Error occurred " << GetLastSocketError(sockfd_.get()) << " on " << name_;
}

}   // namespace network
