//
// Kingsley Chen
//

#include <span>
#include <system_error>

#include "asio/co_spawn.hpp"
#include "asio/detached.hpp"
#include "asio/io_context.hpp"
#include "asio/signal_set.hpp"
#include "asio/this_coro.hpp"
#include "asio/use_awaitable.hpp"
#include "esl/scope_guard.h"
#include "fmt/format.h"
#include "gflags/gflags.h"
#include "spdlog/cfg/env.h"
#include "spdlog/spdlog.h"

#include "chat/chat_server.h"

DEFINE_uint32(port, 9876, "listening port");

template<typename F>
asio::awaitable<void> handle_signals(std::span<int> sigs, F fn) {
    auto ex = co_await asio::this_coro::executor;
    asio::signal_set signals(ex);
    for (auto sig : sigs) {
        signals.add(sig);
    }

    while (true) {
        auto sig = co_await signals.async_wait(asio::use_awaitable);
        fn(sig);
    }
}

void initialize_logger() {
    spdlog::cfg::load_env_levels();
    auto logger = spdlog::default_logger();
    logger->flush_on(spdlog::level::info);
}

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, false);
    initialize_logger();
    ESL_ON_SCOPE_EXIT {
        spdlog::shutdown();
    };

    try {
        asio::io_context io_ctx;

#if defined(_WIN32)
        auto signals = std::array{SIGINT, SIGTERM};
#else
        auto signals = std::array{SIGINT, SIGTERM, SIGHUP};
#endif
        asio::co_spawn(io_ctx,
                       handle_signals(std::span{signals}, [&io_ctx](int sig) {
                           if (sig == SIGINT || sig == SIGTERM) {
                               SPDLOG_INFO("signal={} is caught, stop main io_context...", sig);
                               io_ctx.stop();
                           }
                       }),
                       asio::detached);

        chat_server srv(io_ctx, FLAGS_port);
        srv.start();

        io_ctx.run();
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("Unexpected error occured: {}", ex.what());
        return 1;
    }

    SPDLOG_INFO("chat server stopped");

    return 0;
}
