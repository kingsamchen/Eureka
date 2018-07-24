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

void TCPConnection::HandleRead()
{
    constexpr size_t kBufSize = 255;
    char buf[kBufSize];
    ssize_t n = read(conn_channel_.fd(), buf, kBufSize);

    if (n > 0) {
        if (message_handler_) {
            message_handler_(shared_from_this(), buf, static_cast<size_t>(n));
        }
    } else if (n == 0) {
        HandleClose();
    } else {
        HandleError();
    }
}

void TCPConnection::HandleClose()
{
    ENSURE(CHECK, state_ == State::Connected)(kbase::enum_cast(state_)).Require();

    conn_channel_.DisableAll();

    close_handler_(shared_from_this());
}

void TCPConnection::HandleError()
{
    LOG(WARNING) << "Error occurred " << GetLastSocketError(sockfd_.get()) << " on " << name_;
}

}   // namespace network
