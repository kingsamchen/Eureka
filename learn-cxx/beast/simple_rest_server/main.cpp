//
// Kingsley Chen
//

#include <utility>

#include "boost/asio/as_tuple.hpp"
#include "boost/asio/awaitable.hpp"
#include "boost/asio/co_spawn.hpp"
#include "boost/asio/detached.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/address.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/signal_set.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "fmt/format.h"
#include "gflags/gflags.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

DEFINE_uint32(port, 9876, "listening port");

namespace {

asio::awaitable<void> handle_signals(asio::io_context& ioc) { // NOLINT(*-avoid-reference-coroutine-parameters)
    asio::signal_set signals(ioc, SIGINT, SIGTERM);
    co_await signals.async_wait(asio::use_awaitable);
    fmt::println(stderr, "Terminating...");
    ioc.stop();
}

} // namespace

class listener {
public:
    listener(asio::io_context& ioc, asio::ip::tcp::endpoint ep)
        : ioc_(ioc),
          endpoint_(std::move(ep)) {}

    ~listener() = default;

    listener(const listener&) = delete;
    listener(listener&&) = delete;
    listener& operator=(const listener&) = delete;
    listener& operator=(listener&&) = delete;

    asio::awaitable<void> listen() {
        asio::ip::tcp::acceptor acceptor(ioc_, endpoint_, true);
        fmt::println(stderr, "listening...");
        for (;;) {
            try {
                auto sock = co_await acceptor.async_accept(asio::use_awaitable);
                asio::co_spawn(ioc_, serve(std::move(sock)), asio::detached);
            } catch (const std::exception& ex) {
                fmt::println(stderr,
                             "Unexpected error for handling http connection; ex={}",
                             ex.what());
            }
        }
    }

    asio::awaitable<void> serve(asio::ip::tcp::socket sock) {
        beast::flat_buffer buf;
        http::request<http::string_body> req;

        fmt::println(stderr, "New client at {}", sock.remote_endpoint().address().to_string());

        for (;;) {
            co_await http::async_read(sock, buf, req, asio::use_awaitable);
            auto resp = handle_request(req);
            co_await http::async_write(sock, resp, asio::use_awaitable);
        }
    }

    http::response<http::string_body> handle_request(const http::request<http::string_body>& req) {
        if (req.method() == http::verb::get && req.target() == "/ping") {
            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::server, "Beast");
            res.set(http::field::content_type, "text/plain");
            res.keep_alive(req.keep_alive());
            res.body() = "Hello, World!";
            res.prepare_payload();
            return res;
        }

        auto bad_res = http::response<http::string_body>{http::status::bad_request, req.version()};
        bad_res.keep_alive(req.keep_alive());
        bad_res.prepare_payload();
        return bad_res;
    }

private:
    asio::io_context& ioc_;
    asio::ip::tcp::endpoint endpoint_;
};

int main() {
    try {
        asio::io_context ioc(1);

        asio::co_spawn(ioc, handle_signals(ioc), asio::detached);

        listener listener(ioc, asio::ip::tcp::endpoint(
                                       asio::ip::address{},
                                       static_cast<asio::ip::port_type>(FLAGS_port)));
        asio::co_spawn(ioc, listener.listen(), asio::detached);

        ioc.run();
    } catch (const std::exception& ex) {
        fmt::println(stderr, "Unhandled exception={}", ex.what());
        return 1;
    }

    return 0;
}
