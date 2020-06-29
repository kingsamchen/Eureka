/*
 @ 0xCCCCCCCC
*/

#include "tunnel.h"

#include "kbase/logging.h"

Tunnel::Tunnel(Tunnel::Passkey, asio::io_context::executor_type executor, tcp::socket&& client_sock)
    : executor_(executor),
      client_sock_(std::move(client_sock))
{}

// static
std::shared_ptr<Tunnel> Tunnel::Make(asio::io_context::executor_type executor,
                                     tcp::socket&& client_sock)
{
    return std::make_shared<Tunnel>(Passkey{}, executor, std::move(client_sock));
}

void Tunnel::Run()
{
    LOG(INFO) << "Tunnel is running for client from " << client_sock_.remote_endpoint();
}
