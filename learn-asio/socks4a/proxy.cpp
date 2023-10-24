/*
 @ 0xCCCCCCCC
*/

#include "proxy.h"

#include "glog/logging.h"

#include "tunnel.h"

Proxy::Proxy(asio::io_context& ctx, tcp::endpoint& addr)
    : io_ctx_(ctx),
      acceptor_(ctx, addr, true)
{}

void Proxy::Start()
{
    LOG(INFO) << "Proxy is running on " << acceptor_.local_endpoint();
    DoAccept();
}

void Proxy::DoAccept()
{
     acceptor_.async_accept([this](std::error_code ec, tcp::socket sock) {
         if (ec) {
             LOG(ERROR) << "Failed to accept new connection; ec=" << ec;
         } else {
             LOG(INFO) << "New client connected @ " << sock.remote_endpoint();
             auto tunnel = std::make_shared<Tunnel>(io_ctx_, std::move(sock));
             tunnel->Run();
         }

         DoAccept();
     });
}
