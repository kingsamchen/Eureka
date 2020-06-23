/*
 @ 0xCCCCCCCC
*/

#ifndef LEARN_ASIO_SOCKS5_PROXY_H_
#define LEARN_ASIO_SOCKS5_PROXY_H_

#include <cstdint>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"

#include "kbase/basic_macros.h"

using asio::ip::tcp;

class Proxy {
public:
    Proxy(asio::io_context& ctx, uint16_t port);

    ~Proxy() = default;

    DISALLOW_COPY(Proxy);

    DISALLOW_MOVE(Proxy);

    void Start();

    void Stop();

private:
    asio::io_context& io_ctx_;
    tcp::endpoint listen_addr_;
    tcp::acceptor acceptor_;
};

#endif  // LEARN_ASIO_SOCKS5_PROXY_H_
