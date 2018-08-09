/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include "kbase/logging.h"

namespace network {

using namespace std::placeholders;

TCPConnection::TCPConnection(EventLoop* loop, const std::string& name, ScopedSocket&& sock,
                             const SocketAddress& local_addr, const SocketAddress& peer_addr)
    : loop_(loop),
      name_(name),
      socket_(std::move(sock)),
      sock_notifier_(loop, socket_.get()),
      state_(State::Connecting),
      local_addr_(local_addr),
      peer_addr_(peer_addr),
      read_ctx_(IOEvent::ReadProbe),
      write_ctx_(IOEvent::Write),
      oustanding_writes_(0)
{
    sock_notifier_.SetReadCompleteEvent(
        std::bind(&TCPConnection::HandleReadCompletion, this, _1));
    sock_notifier_.SetWriteCompleteEvent(
        std::bind(&TCPConnection::HandleWriteCompletion, this, _1));
    sock_notifier_.SetCloseEvent(
        std::bind(&TCPConnection::HandleClose, this, _1));
}

TCPConnection::~TCPConnection()
{
    LOG(INFO) << "Destruction of [" << name_ << "] at " << this << " with fd = " << socket_.get();
}

void TCPConnection::EstablishConnection()
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();
    ENSURE(CHECK, state_ == State::Connecting)(kbase::enum_cast(state_)).Require();

    set_state(State::Connected);

    sock_notifier_.EnableNotification();

    if (connection_handler_) {
        connection_handler_(shared_from_this());
    }

    PostReadProbe();
}

void TCPConnection::DestroyConnection()
{
    set_state(State::Disconnected);

    connection_handler_(shared_from_this());
}

void TCPConnection::Send(const std::string& msg)
{
    if (state_ != State::Connected) {
        return;
    }

    loop_->RunTaskInLoop([this, msg] {
        SendInLoop(msg);
    });
}

void TCPConnection::SendInLoop(const std::string& msg)
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();
    output_buf_.Write(msg.data(), msg.size());
    LOG(INFO) << "output-buffer write " << msg.size() << " bytes";
    if (oustanding_writes_ == 0) {
        PostWrite();
    }
}

void TCPConnection::Shutdown()
{
    if (state_ == State::Connected) {
        state_ = State::Disconnecting;
        loop_->RunTaskInLoop([this] { ShutdownInLoop(); });
    }
}

void TCPConnection::ShutdownInLoop()
{
    // Shutdown sending only when all outstanding writes complete.
    if (oustanding_writes_ == 0) {
        shutdown(socket_.get(), SD_SEND);
    }
}

void TCPConnection::HandleReadCompletion(IOContext* io_ctx)
{
    if (io_ctx->io_event == IOEvent::ReadProbe) {
        read_ctx_.io_event = IOEvent::Read;
    } else {
        if (io_ctx->bytes_transferred == 0) {
            HandleClose(io_ctx);
            return;
        }

        input_buf_.EndWrite(io_ctx->bytes_transferred);
    }

    if (message_handler_) {
        message_handler_(shared_from_this(), input_buf_);
    }

    PostRead();
}

void TCPConnection::HandleWriteCompletion(IOContext* io_ctx)
{
    --oustanding_writes_;
    LOG(INFO) << "output-buffer consume " << io_ctx->bytes_transferred << " bytes";
    output_buf_.Consume(io_ctx->bytes_transferred);
    LOG(INFO) << "output-buffer readable size " << output_buf_.readable_size() << " bytes";
    if (output_buf_.readable_size() > 0) {
        PostWrite();
    } else if (state_ == State::Disconnecting) {
        ShutdownInLoop();
    }
}

void TCPConnection::HandleClose(IOContext* io_ctx)
{
    ENSURE(CHECK, state_ == State::Connected || state_ == State::Disconnecting)
        (kbase::enum_cast(state_)).Require();
    close_handler_(shared_from_this());
}

void TCPConnection::PostReadProbe()
{
    DWORD flags = 0;
    WSABUF buf {};
    int rv = WSARecv(socket_.get(), &buf, 1, nullptr, &flags, &read_ctx_, nullptr);
    if (rv != 0 && WSAGetLastError() != WSA_IO_PENDING) {
        LOG(WARNING) << "Failed to issue read probing : " << WSAGetLastError();
    }
}

void TCPConnection::PostRead()
{
    ENSURE(CHECK, read_ctx_.io_event == IOEvent::Read).Require();

    DWORD flags = 0;
    WSABUF buf {};
    buf.buf = input_buf_.BeginWrite();
    buf.len = static_cast<ULONG>(input_buf_.writable_size());

    read_ctx_.Reset();

    int rv = WSARecv(socket_.get(), &buf, 1, nullptr, &flags, &read_ctx_, nullptr);
    if (rv != 0 && WSAGetLastError() != WSA_IO_PENDING) {
        LOG(WARNING) << "Failed to issue read probing : " << WSAGetLastError();
    }
}

void TCPConnection::PostWrite()
{
    ++oustanding_writes_;

    WSABUF buf {};
    buf.buf = const_cast<char*>(output_buf_.Peek());
    buf.len = static_cast<ULONG>(output_buf_.readable_size());

    write_ctx_.Reset();

    int rv = WSASend(socket_.get(), &buf, 1, nullptr, 0, &write_ctx_, nullptr);
    if (rv != 0 && WSAGetLastError() != WSA_IO_PENDING) {
        LOG(WARNING) << "Failed to issue read probing : " << WSAGetLastError();
    }
}

}   // namespace network
