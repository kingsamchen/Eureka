//
// 0xCCCCCCCC
//

#include <array>
#include <cstdint>
#include <exception>

#include "asio/as_tuple.hpp"
#include "asio/awaitable.hpp"
#include "asio/buffer.hpp"
#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/experimental/awaitable_operators.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/signal_set.hpp"
#include "asio/system_timer.hpp"
#include "asio/use_awaitable.hpp"
#include "asio/write.hpp"
#include "esl/scope_guard.h"
#include "fmt/format.h"
#include "fmt/os.h"
#include "fmt/ostream.h"
#include "gflags/gflags.h"
#include "spdlog/spdlog.h"

using sys_clock = std::chrono::system_clock;

using namespace asio::experimental::awaitable_operators;

asio::awaitable<void> echo(asio::ip::tcp::socket& sock, sys_clock::time_point& deadline) {
    ESL_ON_SCOPE_EXIT {
        SPDLOG_INFO("return from echo");
    };

    auto rep = sock.remote_endpoint();
    try {
        std::array<char, 1024> data;
        while (true) {
            deadline = sys_clock::now() + std::chrono::seconds(5);
            std::size_t len_read =
                    co_await sock.async_read_some(asio::buffer(data), asio::use_awaitable);
            co_await asio::async_write(sock, asio::buffer(data, len_read), asio::use_awaitable);
        }
    } catch (const asio::system_error& ex) {
        SPDLOG_ERROR("Unhandled asio system error={} on conn={}", ex.what(), fmt::streamed(rep));
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("Unhandled exception={} on conn={}", ex.what(), fmt::streamed(rep));
    }
}

asio::awaitable<void> watchdog(asio::ip::tcp::socket& sock, const sys_clock::time_point& deadline) {
    ESL_ON_SCOPE_EXIT {
        SPDLOG_INFO("return from watchdog");
    };

    asio::system_timer idle_timer(co_await asio::this_coro::executor);
    while (deadline > sys_clock::now()) {
        idle_timer.expires_at(deadline);
        co_await idle_timer.async_wait(asio::use_awaitable);
    }

    auto rep = sock.remote_endpoint();
    SPDLOG_INFO("Conn[{}] idled timeout", fmt::streamed(rep));

    // Shutdown read will cause echo read EOF from async read.
    // NOTE: nc on Linux won't send back FIN after receiving  shutdown write.
    std::error_code ec;
    sock.shutdown(asio::ip::tcp::socket::shutdown_send, ec);
    if (ec) {
        SPDLOG_WARN("shutdown ec={}", fmt::streamed(ec));
    }
    // ec.clear();
    // sock.cancel(ec);

    // Simulate lingering to make sure echo has enough time to finish.
    idle_timer.expires_after(std::chrono::seconds(3));
    co_await idle_timer.async_wait(asio::use_awaitable);
}

asio::awaitable<void> handle_connection(asio::ip::tcp::socket sock) {
    auto rep = sock.remote_endpoint();
    SPDLOG_INFO("Conn[{}] connected", fmt::streamed(rep));

    sys_clock::time_point deadline{};
    co_await (echo(sock, deadline) || watchdog(sock, deadline));

    SPDLOG_INFO("Conn[{}] disconnected", fmt::streamed(rep));
}

asio::awaitable<void> listener(std::uint16_t port) {
    auto executor = co_await asio::this_coro::executor;
    asio::ip::tcp::acceptor acceptor(executor, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
    while (true) {
        auto [ec, sock] = co_await acceptor.async_accept(asio::as_tuple(asio::use_awaitable));
        if (ec) {
            SPDLOG_ERROR("Failed to accept new conn; ec={}", fmt::streamed(ec));
            continue;
        }
        asio::co_spawn(executor, handle_connection(std::move(sock)), asio::detached);
    }
}

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    try {
        asio::io_context io_ctx;

        asio::signal_set signals(io_ctx, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) {
            io_ctx.stop();
            fmt::println("bye-bye");
        });

        asio::co_spawn(io_ctx, listener(9876), asio::detached);
        SPDLOG_INFO("echo server is running");

        io_ctx.run();
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("Unhandled exception={}", ex.what());
        return 1;
    }

    return 0;
}
