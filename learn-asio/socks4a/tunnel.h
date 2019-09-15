/*
 @ 0xCCCCCCCC
*/

#include <memory>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"

#include "kbase/basic_macros.h"

using asio::ip::tcp;

class Tunnel : public std::enable_shared_from_this<Tunnel> {
public:
    Tunnel(asio::io_context& ctx, tcp::socket&& client_sock);

    ~Tunnel() = default;

    DISALLOW_COPY(Tunnel);

    DISALLOW_MOVE(Tunnel);

private:
    asio::io_context& io_ctx_;
    tcp::socket client_sock_;
    tcp::socket dial_sock_;
    tcp::socket remote_sock_;
};