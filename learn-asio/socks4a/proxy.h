/*
 @ 0xCCCCCCCC
*/

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"

#include "kbase/basic_macros.h"

using asio::ip::tcp;

class Proxy {
public:
    Proxy(asio::io_context& ctx, tcp::endpoint& addr);

    ~Proxy() = default;

    DISALLOW_COPY(Proxy);

    DISALLOW_MOVE(Proxy);

    void Start();

private:
    void DoAccept();

private:
    asio::io_context& io_ctx_;
    tcp::acceptor acceptor_;
};
