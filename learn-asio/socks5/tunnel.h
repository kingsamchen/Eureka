/*
 @ 0xCCCCCCCC
*/

#ifndef LEARN_ASIO_SOCKS5_TUNNEL_H_
#define LEARN_ASIO_SOCKS5_TUNNEL_H_

#include <memory>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"

#include "kbase/basic_macros.h"

using asio::ip::tcp;

class Tunnel : public std::enable_shared_from_this<Tunnel> {
private:
    class Passkey {
        Passkey() noexcept {}
        friend Tunnel;
    };

public:
    Tunnel(Passkey, asio::io_context::executor_type executor, tcp::socket&& client_sock);

    ~Tunnel() = default;

    DISALLOW_COPY(Tunnel);

    DISALLOW_MOVE(Tunnel);

    static std::shared_ptr<Tunnel> Make(asio::io_context::executor_type executor, 
                                        tcp::socket&& client_sock);

    void Run();

private:
    asio::io_context::executor_type executor_;
    tcp::socket client_sock_;
};

#endif  // LEARN_ASIO_SOCKS5_TUNNEL_H_
