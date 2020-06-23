/*
 @ 0xCCCCCCCC
*/

#include "proxy.h"

Proxy::Proxy(asio::io_context& ctx, uint16_t port)
    : io_ctx_(ctx),
      listen_addr_(tcp::v4(), port),
      acceptor_(io_ctx_, listen_addr_, true)
{}

void Proxy::Start()
{}

void Proxy::Stop()
{}
