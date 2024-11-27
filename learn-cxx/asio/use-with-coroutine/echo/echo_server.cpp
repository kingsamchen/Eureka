//
// 0xCCCCCCCC
//

#include <array>
#include <cstdint>
#include <exception>
#include <chrono>

#include "asio/awaitable.hpp"
#include "asio/buffer.hpp"
#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/signal_set.hpp"
#include "asio/use_awaitable.hpp"
#include "asio/write.hpp"
#include "fmt/format.h"
#include "fmt/os.h"
#include "fmt/ostream.h"
#include "gflags/gflags.h"

asio::awaitable<void> echo(asio::ip::tcp::socket sock) {
    try {
        std::array<char, 1024> data;
        while (true) {
            std::size_t len_read =
                    co_await sock.async_read_some(asio::buffer(data), asio::use_awaitable);
            co_await asio::async_write(sock, asio::buffer(data, len_read), asio::use_awaitable);
        }
    } catch (const asio::system_error& ex) {
        auto rep = sock.remote_endpoint();
        if (ex.code() == asio::error::eof) {
            fmt::println(stderr, "Conn={}:{} disconnected", rep.address().to_string(), rep.port());
        } else {
            fmt::println(stderr, "Unhandled asio system error={} on conn={}:{}",
                         ex.what(), rep.address().to_string(), rep.port());
        }
    } catch (const std::exception& ex) {
        auto rep = sock.remote_endpoint();
        fmt::println(stderr, "Unhandled exception={} on conn={}:{}",
                     ex.what(), rep.address().to_string(), rep.port());
    }
}

asio::awaitable<void> listener(std::uint16_t port) {
    auto executor = co_await asio::this_coro::executor;
    asio::ip::tcp::acceptor acceptor(executor, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
    while (true) {
        auto sock = co_await acceptor.async_accept(asio::use_awaitable);
        asio::co_spawn(executor, echo(std::move(sock)), asio::detached);
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

        io_ctx.run();
    } catch (const std::exception& ex) {
        fmt::println(stderr, "Unhandled exception={}", ex.what());
        return 1;
    }

    return 0;
}
