#include <chrono>
#include <system_error>
#include <thread>

#include <asio.hpp>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

namespace {

asio::awaitable<int> make_awaitable(int& i) {
    SPDLOG_INFO("In make_awaitable");
    //++i;
    co_return 42;
}

asio::awaitable<void> launcher() {
    SPDLOG_INFO("Before lambda");
    auto res = []() -> asio::awaitable<int> {
        int i = 42;
        SPDLOG_INFO("Before make_awaitable");
        auto r = make_awaitable(i);
        SPDLOG_INFO("After make_awaitable");
        return r;
    }();
    SPDLOG_INFO("After lambda");
    auto n = co_await std::move(res);
    co_return;
}

} // namespace

asio::awaitable<void> run_custom_async();

int main() {
    asio::io_context io_ctx{1};

    // asio::co_spawn(io_ctx, launcher(), asio::detached);
    // asio::co_spawn(io_ctx, timer(), asio::detached);
    // asio::co_spawn(io_ctx, run_custom_async(), asio::detached);
    io_ctx.run();
    return 0;
}
