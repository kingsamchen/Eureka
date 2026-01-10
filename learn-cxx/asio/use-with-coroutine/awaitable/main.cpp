#include <chrono>
#include <system_error>
#include <thread>

#include <asio.hpp>
#include <fmt/format.h>
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

struct Tracker { // NOLINT(cppcoreguidelines-special-member-functions)
    int val;
    explicit Tracker(int v) : val(v) {}
    ~Tracker() {
        SPDLOG_INFO("~Tracker");
    }
};

int format_as(const Tracker& tracker) { // NOLINT(performance-unnecessary-value-param)
    return tracker.val;
}

Tracker get_value() {
    return Tracker(42);
}

asio::awaitable<void> capture_lambda() {
    auto value = get_value();
    asio::awaitable<void> co;
    {
        auto lambda = [value]() -> asio::awaitable<void> {
            co_await asio::this_coro::executor;
            SPDLOG_INFO("the value={}", value);
            // "value" have already been destroyed
        };
        co = lambda();
    } // the lambda closure object has now gone out of scope
    SPDLOG_INFO("about to co_await");
    co_await std::move(co);
}

} // namespace

asio::awaitable<void> run_custom_async();

int main() {
    asio::io_context io_ctx{1};

    // asio::co_spawn(io_ctx, launcher(), asio::detached);
    // asio::co_spawn(io_ctx, timer(), asio::detached);
    // asio::co_spawn(io_ctx, run_custom_async(), asio::detached);
    // asio::co_spawn(io_ctx, capture_lambda(), asio::detached);

    io_ctx.run();
    return 0;
}
