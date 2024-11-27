//
// Kingsley Chen
//

#include "chat/chat_server.h"

#include <cassert>

#include "asio/as_tuple.hpp"
#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/steady_timer.hpp"
#include "asio/this_coro.hpp"
#include "asio/use_awaitable.hpp"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "spdlog/spdlog.h"

#include "chat/session.h"

using asio::ip::tcp;

chat_server::chat_server(asio::io_context& ioc, std::uint16_t port)
    : io_ctx_(ioc),
      port_(port) {}

chat_server::~chat_server() {
    SPDLOG_DEBUG("in ~chat_server");
}

void chat_server::start() {
    asio::co_spawn(io_ctx_, listen_and_serve(), asio::detached);
    asio::co_spawn(io_ctx_, on_tick(), asio::detached);
    SPDLOG_INFO("chat server starts running");
}

asio::awaitable<void> chat_server::listen_and_serve() {
    auto ex = co_await asio::this_coro::executor;
    tcp::acceptor acceptor(ex, tcp::endpoint{asio::ip::address_v4{}, port_}, true);

    while (true) {
        auto [ec, sock] = co_await acceptor.async_accept(asio::as_tuple(asio::use_awaitable));
        if (ec) {
            SPDLOG_ERROR("Failed to accept new sock; ec={}", fmt::streamed(ec));
            continue;
        }

        auto session = session::make(std::move(sock));

        session->set_on_message([this](const session::ptr& session, std::string_view msg) {
            broadcast_message(msg);
        });

        session->set_on_close([this](const session::ptr& session) {
            remove_session(session);
        });

        session->start();
        sessions_.insert(session);

        SPDLOG_DEBUG("conn[{}] is added to server's session set",
                     fmt::streamed(session->remote_addr()));
    }
}

asio::awaitable<void> chat_server::on_tick() {
    asio::steady_timer timer(co_await asio::this_coro::executor);
    constexpr auto interval = std::chrono::seconds(3);
    while (true) {
        (void)timer.expires_after(interval);
        co_await timer.async_wait(asio::use_awaitable);
        SPDLOG_DEBUG("# of sessions = {}", sessions_.size());
    }
}

void chat_server::broadcast_message(std::string_view msg) {
    for (auto& s : sessions_) {
        s->send(msg);
    }
}

void chat_server::remove_session(const session::ptr& session) {
    sessions_.erase(session);
    SPDLOG_DEBUG("conn[{}] is removed from server's session set",
                 fmt::streamed(session->remote_addr()));
}
