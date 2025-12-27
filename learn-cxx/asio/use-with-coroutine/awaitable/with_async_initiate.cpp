#include <utility>

#include <asio.hpp>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

int heavy_computation() {
    SPDLOG_INFO("in thread={}", std::this_thread::get_id());
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 42;
}

template<asio::completion_token_for<int(std::error_code, int)> CompletionToken>
auto async_do_heavy_computation(CompletionToken&& token) { // NOLINT
    auto initiation = [](asio::completion_token_for<int(std::error_code, int)> auto&& handler) {
        std::thread th([h = std::move(handler)]() mutable {
            auto ret = heavy_computation();
            std::error_code ec;
            auto executor = asio::get_associated_executor(h);
            asio::post(executor,
                       [h = std::move(h), ec, ret]() mutable {
                           std::move(h)(ec, ret);
                       });
        });
        th.detach();
    };

    return asio::async_initiate<CompletionToken, void(std::error_code, int)>(
            initiation, token);
}

asio::awaitable<void> run_custom_async() {
    SPDLOG_INFO("run_custom_async() in thread={}", std::this_thread::get_id());
    auto ret = co_await async_do_heavy_computation(asio::deferred);
    SPDLOG_INFO("with value={} tid={}", ret, std::this_thread::get_id());
}
