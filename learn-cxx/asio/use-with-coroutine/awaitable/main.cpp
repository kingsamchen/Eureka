#include <asio.hpp>
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

int main() {
    asio::io_context io_ctx{1};

    asio::co_spawn(io_ctx, launcher(), asio::detached);

    io_ctx.run();
    return 0;
}
