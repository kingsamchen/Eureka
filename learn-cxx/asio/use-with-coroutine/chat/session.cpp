//
// Kingsley Chen
//

#include "chat/session.h"

#include <chrono>

#include "asio/buffer.hpp"
#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/read_until.hpp"
#include "asio/redirect_error.hpp"
#include "asio/this_coro.hpp"
#include "asio/write.hpp"
#include "esl/scope_guard.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "spdlog/spdlog.h"

session::session(passkey, asio::ip::tcp::socket&& sock)
    : sock_(std::move(sock)),
      remote_endpoint_(sock_.remote_endpoint()),
      wait_on_msg_(sock_.get_executor(), std::chrono::steady_clock::time_point::max()) {}

session::~session() {
    SPDLOG_DEBUG("conn[{}] is in ~session", fmt::streamed(remote_addr()));
}

// static
session::ptr session::make(asio::ip::tcp::socket&& sock) {
    auto s = std::make_shared<session>(passkey{}, std::move(sock));
    return s;
}

void session::start() {
    auto ex = sock_.get_executor();

    asio::co_spawn(
            ex,
            [self = shared_from_this()] {
                return self->read();
            },
            [self = shared_from_this()](std::exception_ptr eptr) {
                self->handle_error(eptr, "reader");
            });
    asio::co_spawn(
            ex,
            [self = shared_from_this()] {
                return self->write();
            },
            [self = shared_from_this()](std::exception_ptr eptr) {
                self->handle_error(eptr, "writer");
            });

    SPDLOG_DEBUG("conn[{}] starts running", fmt::streamed(remote_addr()));
}

asio::awaitable<void> session::read() {
    ESL_ON_SCOPE_EXIT {
        SPDLOG_DEBUG("conn[{}] returned from reader", fmt::streamed(remote_addr()));
    };

    constexpr size_t max_buf_size = 1024;
    std::string msg_buf;
    while (!closed_) {
        auto msg_cnt = co_await asio::async_read_until(
                sock_,
                asio::dynamic_buffer(msg_buf, max_buf_size),
                '\n',
                asio::use_awaitable);
        if (on_message_) {
            on_message_(shared_from_this(), std::string_view{msg_buf}.substr(0, msg_cnt));
        }
        msg_buf.erase(0, msg_cnt);
    }
}

asio::awaitable<void> session::write() {
    ESL_ON_SCOPE_EXIT {
        SPDLOG_DEBUG("conn[{}] returned from writer", fmt::streamed(remote_addr()));
    };

    while (!closed_) {
        if (msgs_.empty()) {
            std::error_code ec;
            co_await wait_on_msg_.async_wait(asio::redirect_error(asio::use_awaitable, ec));
        } else {
            auto msg = std::move(msgs_.front());
            msgs_.pop_front();
            (void)co_await asio::async_write(sock_, asio::buffer(msg), asio::use_awaitable);
        }
    }
}

void session::send(std::string_view msg) {
    msgs_.push_back(std::string{msg});
    wait_on_msg_.cancel_one(); // wakeup if write is in sleep.
}

void session::handle_close() noexcept {
    SPDLOG_DEBUG("conn[{}] is in handle_close(); closed={}", fmt::streamed(remote_addr()), closed_);

    if (closed_) {
        return;
    }

    closed_ = true;

    std::error_code ec;
    wait_on_msg_.cancel(ec);
    sock_.cancel(ec);
    if (ec) {
        SPDLOG_WARN("Error during cancel; ec=", fmt::streamed(ec));
    }

    auto self = shared_from_this();
    on_close_(self);
}

void session::handle_error(std::exception_ptr eptr, std::string_view extra) noexcept {
    if (!eptr) {
        return;
    }

    try {
        std::rethrow_exception(eptr);
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("conn[{}] {} disconnected due to {}",
                     fmt::streamed(remote_addr()), extra, ex.what());
        handle_close();
    }
}
