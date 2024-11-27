//
// Kingsley Chen
//

#pragma once

#include <cstdint>
#include <unordered_set>

#include "asio/awaitable.hpp"
#include "asio/io_context.hpp"

#include "chat/session.h"

class chat_server {
public:
    chat_server(asio::io_context& ioc, std::uint16_t port);

    ~chat_server();

    chat_server(const chat_server&) = delete;

    chat_server(chat_server&&) = delete;

    chat_server& operator=(const chat_server&) = delete;

    chat_server& operator=(chat_server&&) = delete;

    void start();

private:
    asio::awaitable<void> listen_and_serve();

    asio::awaitable<void> on_tick();

    void broadcast_message(std::string_view msg);

    void remove_session(const session::ptr& session);

private:
    asio::io_context& io_ctx_;
    std::uint16_t port_;
    std::unordered_set<session::ptr> sessions_;
};
