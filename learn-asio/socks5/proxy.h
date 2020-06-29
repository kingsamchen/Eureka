/*
 @ 0xCCCCCCCC
*/

#ifndef LEARN_ASIO_SOCKS5_PROXY_H_
#define LEARN_ASIO_SOCKS5_PROXY_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"

#include "kbase/basic_macros.h"

#include "event_thread.h"

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
    asio::io_context::executor_type PickExecutor() noexcept;

    void DoAccept();

private:
    asio::io_context& io_ctx_;
    tcp::endpoint listen_addr_;
    tcp::acceptor acceptor_;
    std::vector<std::unique_ptr<EventThread>> worker_pool_;
    size_t next_worker_id_;
};

#endif  // LEARN_ASIO_SOCKS5_PROXY_H_
