//
// Kingsley Chen
//

#pragma once

#include <deque>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "asio/awaitable.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/steady_timer.hpp"

class session : public std::enable_shared_from_this<session> {
    struct passkey {
        explicit passkey() = default;
    };

public:
    using ptr = std::shared_ptr<session>;

    using on_message_handler = std::function<void(const ptr&, std::string_view)>;

    using on_close_handler = std::function<void(const ptr&)>;

    session(passkey, asio::ip::tcp::socket&& sock);

    ~session();

    session(const session&) = delete;

    session& operator=(const session&) = delete;

    static ptr make(asio::ip::tcp::socket&& sock);

    void start();

    void send(std::string_view msg);

    void set_on_message(const on_message_handler& handler) {
        on_message_ = handler;
    }

    void set_on_close(const on_close_handler& handler) {
        on_close_ = handler;
    }

    const asio::ip::tcp::endpoint& remote_addr() const noexcept {
        return remote_endpoint_;
    }

private:
    asio::awaitable<void> read();

    asio::awaitable<void> write();

    void handle_close() noexcept;

    void handle_error(std::exception_ptr eptr, std::string_view extra) noexcept;

private:
    bool closed_{false};
    asio::ip::tcp::socket sock_;
    asio::ip::tcp::endpoint remote_endpoint_;
    asio::steady_timer wait_on_msg_; // used to wait-on/wakeup write coroutine.
    std::deque<std::string> msgs_;
    on_message_handler on_message_;
    on_close_handler on_close_;
};
