/*
 @ 0xCCCCCCCC
*/

#include "acceptor.h"

#include "kbase/error_exception_util.h"

#include "event_loop.h"
#include "winsock_context.h"

namespace {

using namespace std::placeholders;

using network::ScopedSocket;

ScopedSocket CreateSocket()
{
    ScopedSocket listener(
        WSASocketW(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED));
    ENSURE(CHECK, !!listener)(WSAGetLastError()).Require();
    return listener;
}

}   // namespace

namespace network {

Acceptor::Acceptor(EventLoop* loop, const SocketAddress& addr)
    : loop_(loop),
      server_addr_(addr),
      listenning_(false),
      listener_(CreateSocket()),
      listener_notifier_(loop, listener_.get()),
      accept_conn_(CreateSocket()),
      accept_ctx_(IOEvent::Read)
{
    // Enable reuse of address.
    char optval = 1;
    //int rv = setsockopt(listener_.get(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    //ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    int rv = setsockopt(listener_.get(), SOL_SOCKET, SO_EXCLUSIVEADDRUSE, &optval, sizeof(optval));
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    const auto& sock_addr = server_addr_.raw();
    rv = bind(listener_.get(), reinterpret_cast<const sockaddr*>(&sock_addr), sizeof(sock_addr));
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    listener_notifier_.SetReadCompleteEvent(std::bind(&Acceptor::HandleAcceptCompletion, this, _1));
}

void Acceptor::Listen()
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();

    listenning_ = true;

    int rv = listen(listener_.get(), SOMAXCONN);
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    listener_notifier_.EnableNotification();

    PostAccept();
}

void Acceptor::PostAccept()
{
    memset(accept_addr_block_, 0, kAcceptAddrLength);

    DWORD receive_len = 0;
    WinsockContext::instance().AcceptEx(
        listener_.get(), accept_conn_.get(), accept_addr_block_, 0, kAcceptAddrLength,
        kAcceptAddrLength, &receive_len, &accept_ctx_);
}

void Acceptor::HandleAcceptCompletion(IOContext* io_ctx)
{
    auto listener = listener_.get();
    int rv = setsockopt(accept_conn_.get(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
                        reinterpret_cast<const char*>(&listener), sizeof(SOCKET));
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    sockaddr_in addr {};
    int len = sizeof(addr);
    getpeername(accept_conn_.get(), reinterpret_cast<sockaddr*>(&addr), &len);

    if (new_connection_handler_) {
        new_connection_handler_(std::move(accept_conn_), SocketAddress(addr));
    }

    accept_conn_ = CreateSocket();
    PostAccept();
}

}   // namespace network
