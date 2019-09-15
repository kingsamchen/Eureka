/*
 @ 0xCCCCCCCC
*/

#include "tunnel.h"

Tunnel::Tunnel(asio::io_context& ctx, tcp::socket&& client_sock)
    : io_ctx_(ctx),
      client_sock_(std::move(client_sock)),
      dial_sock_(ctx),
      remote_sock_(ctx)
{
    // TODO:
}
