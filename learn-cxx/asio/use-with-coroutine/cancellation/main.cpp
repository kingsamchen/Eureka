//
// Kingsley Chen
//

#include <list>

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <fmt/chrono.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

namespace asio = boost::asio;

using namespace asio::experimental::awaitable_operators; // NOLINT(google-build-using-namespace)
using namespace std::chrono_literals;

namespace {

asio::awaitable<void> serve() {
    co_await asio::this_coro::reset_cancellation_state(
            asio::enable_total_cancellation(), asio::enable_terminal_cancellation());
    auto cs = co_await asio::this_coro::cancellation_state;
    SPDLOG_INFO("Inside server");

    asio::cancellation_signal sig;
    cs.slot().assign([&sig](asio::cancellation_type type) {
        SPDLOG_INFO("Received type={}", static_cast<unsigned int>(type));
        sig.emit(type);
    });

    asio::steady_timer timer(co_await asio::this_coro::executor);
    timer.expires_at(asio::steady_timer::time_point::max());
    auto [ec] = co_await timer.async_wait(asio::bind_cancellation_slot(sig.slot(), asio::as_tuple));
    if (ec == asio::error::operation_aborted) {
        SPDLOG_INFO("Timer was cancelled");
    } else if (ec) {
        SPDLOG_ERROR("Timer error: {}", ec.message());
    } else {
        SPDLOG_INFO("Timer expired");
    }
    co_return;
}

asio::awaitable<void> launcher() {
    SPDLOG_INFO("Inside launcher");
    static std::list<asio::cancellation_signal> signals;
    auto wrapper = []() -> asio::awaitable<void> {
        auto& it = signals.emplace_back();
        co_await asio::co_spawn(
                co_await asio::this_coro::executor,
                serve(),
                asio::bind_cancellation_slot(it.slot(), asio::deferred));
        co_return;
    };
    auto cs = co_await asio::this_coro::cancellation_state;
    for (int i = 0; i < 3; ++i) {
        asio::co_spawn(co_await asio::this_coro::executor, wrapper(), asio::detached);
    }
    cs.slot().assign([](asio::cancellation_type type) {
        SPDLOG_INFO("Received type={}", static_cast<unsigned int>(type));
        for (auto& sig : signals) {
            sig.emit(type);
        }
    });
}

asio::awaitable<void> timeout_after(std::chrono::seconds dur) {
    asio::steady_timer timer(co_await asio::this_coro::executor);
    timer.expires_after(dur);
    co_await timer.async_wait();
}

auto coro_sleep(int id, std::chrono::seconds dur) -> asio::awaitable<int> {
    SPDLOG_INFO("Entering sleep on thread={}; id={}", std::this_thread::get_id(), id);
    asio::steady_timer timer(co_await asio::this_coro::executor);
    timer.expires_after(dur);
    SPDLOG_INFO("Before sleep; id={}", id);
    co_await timer.async_wait();
    SPDLOG_INFO("Sleep completed; id={} dur={}", id, dur);
    co_return id;
}

auto coro_sleep_with_busy_work(int id, std::chrono::seconds dur, std::chrono::seconds busy_dur)
        -> asio::awaitable<int> {
    SPDLOG_INFO("Entering sleep on thread={}; id={}", std::this_thread::get_id(), id);
    SPDLOG_INFO("simulating cpu work for {} second", busy_dur.count());
    std::this_thread::sleep_for(busy_dur);
    asio::steady_timer timer(co_await asio::this_coro::executor);
    timer.expires_after(dur);
    SPDLOG_INFO("Before sleep; id={}", id);
    co_await timer.async_wait();
    SPDLOG_INFO("Sleep completed; id={} dur={}", id, dur);
    co_return id;

} // namespace

TEST_CASE("Trivial Cancellation Signal/Slot with Handler") {
    asio::cancellation_signal cancel_signal;

    std::thread j([slot = cancel_signal.slot()] mutable {
        std::atomic<bool> quit{false};
        slot.assign([&quit](asio::cancellation_type type) {
            SPDLOG_INFO("Cancel with type={} on thread={}",
                        static_cast<unsigned int>(type), std::this_thread::get_id());
            quit.store(true);
        });

        while (!quit.load()) {
            SPDLOG_INFO("Worker is doing job on thread={}", std::this_thread::get_id());
            std::this_thread::sleep_for(1s);
        }
    });

    std::this_thread::sleep_for(4s);
    SPDLOG_INFO("Emitting cancellation on thread={}", std::this_thread::get_id());
    cancel_signal.emit(asio::cancellation_type_t::all);

    j.join();
}

TEST_CASE("Trivial Cancellation Signal/Slot with Bound AsyncOP") {
    asio::io_context ioc{1};

    asio::cancellation_signal cancel_signal;

    struct worker_op {
        void operator()(std::error_code ec) {
            if (ec) {
                SPDLOG_INFO("{}", ec.message());
                return;
            }
            SPDLOG_INFO("Repeat worker is doing job");
            t.expires_after(1s);
            t.async_wait(asio::bind_cancellation_slot(cancel_slot, *this));
        }
        asio::steady_timer& t;
        asio::cancellation_slot cancel_slot;
    };

    asio::steady_timer repeat_worker(ioc);
    worker_op op{repeat_worker, cancel_signal.slot()}; // NOLINT(modernize-use-designated-initializers)
    repeat_worker.async_wait(asio::bind_cancellation_slot(cancel_signal.slot(), op));

    asio::steady_timer cancel_timer(ioc);
    cancel_timer.expires_after(4s);
    cancel_timer.async_wait([&cancel_signal](std::error_code ec) {
        (void)ec;
        cancel_signal.emit(asio::cancellation_type_t::all);
    });

    ioc.run();
}

template<typename CompletionToken>
auto async_low_wait(asio::steady_timer& timer, CompletionToken&& token) {
    return timer.async_wait(std::forward<CompletionToken>(token));
}

template<typename CompletionToken>
auto async_outer_wait(asio::steady_timer& timer, CompletionToken&& token) {
    auto op = [&timer, started = false](auto& self, boost::system::error_code ec = {}) mutable {
        if (!started) {
            started = true;

            auto parent_slot =
                    asio::get_associated_cancellation_slot(self);

            // Outer layer creates its own state from the caller's slot.
            auto cs = asio::cancellation_state(parent_slot);

            SPDLOG_INFO("[outer] started");

            // Pass the OUTER child slot down to middle.
            async_low_wait(
                    timer,
                    asio::bind_cancellation_slot(
                            cs.slot(),
                            std::move(self)));
            return;
        }

        if (ec == asio::error::operation_aborted) {
            SPDLOG_INFO("[outer] inner layer completed with cancellation");
        }

        self.complete(ec);
    };

    return asio::async_compose<CompletionToken, void(boost::system::error_code)>(
            std::move(op),
            token,
            timer);
}

TEST_CASE("Layered Cancellation with cancellation_state") {
    asio::io_context io;

    asio::steady_timer work_timer(io);
    work_timer.expires_after(10s);

    asio::steady_timer cancel_timer(io);
    cancel_timer.expires_after(2s);

    asio::cancellation_signal sig;

    async_outer_wait(
            work_timer,
            asio::bind_cancellation_slot(
                    sig.slot(),
                    [](boost::system::error_code ec) {
                        std::cout << "[user handler] ec = " << ec.message() << "\n";
                    }));

    cancel_timer.async_wait(
            [&](boost::system::error_code ec) {
                if (!ec) {
                    std::cout << "[caller] emit terminal cancellation\n";
                    sig.emit(asio::cancellation_type::terminal);
                }
            });

    io.run();
}

TEST_CASE("Coroutine Cancellation") {
    asio::io_context ioc{1};
    asio::cancellation_signal cancel_signal;

    auto launch = []() -> asio::awaitable<void> {
        SPDLOG_INFO("Launch work timer");
        co_await []() -> asio::awaitable<void> {
            auto cs = co_await asio::this_coro::cancellation_state;

            SPDLOG_INFO("Work timer starts");
            asio::steady_timer timer(co_await asio::this_coro::executor);
            timer.expires_after(10s);
            auto [ec] = co_await timer.async_wait(
                    asio::bind_cancellation_slot(cs.slot(), asio::as_tuple));
            if (ec) {
                SPDLOG_INFO("Work timer {}", ec.message());
                co_return;
            }
            SPDLOG_INFO("Timer tick");
        }();
    };
    asio::co_spawn(ioc, launch, asio::bind_cancellation_slot(cancel_signal.slot(), asio::detached));

    asio::steady_timer cancel_timer(ioc);
    cancel_timer.expires_after(2s);
    cancel_timer.async_wait([&cancel_signal](boost::system::error_code ec) {
        if (!ec) {
            SPDLOG_INFO("Emit cancellation");
            cancel_signal.emit(asio::cancellation_type::all);
        }
    });

    ioc.run();
}

TEST_CASE("Cancellation Signal/Slot and State") {
    asio::io_context ioc{1};
    asio::cancellation_signal stop_signal;
    asio::co_spawn(ioc, launcher(),
                   asio::bind_cancellation_slot(stop_signal.slot(), asio::detached));
    asio::steady_timer timer(ioc);
    timer.expires_after(std::chrono::seconds(5));
    timer.async_wait([&](const boost::system::error_code& ec) {
        if (!ec) {
            SPDLOG_INFO("Timer expired, requesting stop");
            stop_signal.emit(asio::cancellation_type::all);
        }
    });
    ioc.run();
}

TEST_CASE("Awaitable Timeout Controll") {
    asio::io_context ioc{1};
    auto launch = []() -> asio::awaitable<void> {
        asio::steady_timer timer(co_await asio::this_coro::executor);
        timer.expires_after(std::chrono::seconds(3));
        auto e = co_await (timer.async_wait(asio::use_awaitable) ||
                           timeout_after(std::chrono::seconds{5}));
        SPDLOG_INFO("Result index: {}", e.index());
        co_await timer.async_wait();
        SPDLOG_INFO("Timer expired, exiting");
    };
    asio::co_spawn(ioc, launch(), asio::detached);
    ioc.run();
}

TEST_CASE("Concurrent Awaitables") {
    asio::io_context ioc{1};
    auto launch = []() -> asio::awaitable<void> {
        SPDLOG_INFO("Waiting for multiple concurrent awaitables");
        auto [s1, s2] = co_await (coro_sleep(1, std::chrono::seconds{3}) &&
                                  coro_sleep(2, std::chrono::seconds{5}));
        SPDLOG_INFO("All completed on thread={} with results {} {}", std::this_thread::get_id(), s1, s2);
        co_return;
    };
    asio::co_spawn(ioc, launch(), asio::detached);
    ioc.run();
}

TEST_CASE("Parallel Awaitables") {
    using namespace std::chrono_literals;

    asio::io_context ioc{1};
    asio::thread_pool pool{4};

    // co_await asio::post(pool.get_executor()) is required if posting from the same thread-pool.
    auto launch = [&pool]() -> asio::awaitable<void> { // NOLINT
        SPDLOG_INFO("Entry coroutine launched on thread={}", std::this_thread::get_id());

        auto c1 = asio::co_spawn(pool.get_executor(), [&pool] -> asio::awaitable<int> {
            SPDLOG_INFO("sub-coro-1 launched on thread={}", std::this_thread::get_id());
            // co_await asio::post(pool.get_executor());
            SPDLOG_INFO("sub-coro-1 sleeps on thread={}", std::this_thread::get_id());
            co_return co_await coro_sleep_with_busy_work(1, 3s, 3s); }, asio::use_awaitable);

        auto c2 = asio::co_spawn(pool.get_executor(), [&pool] -> asio::awaitable<int> {
            SPDLOG_INFO("sub-coro-2 launched on thread={}", std::this_thread::get_id());
            // co_await asio::post(pool.get_executor());
            SPDLOG_INFO("sub-coro-2 sleeps on thread={}", std::this_thread::get_id());
            co_return co_await coro_sleep_with_busy_work(2, 5s, 3s); }, asio::use_awaitable);

        SPDLOG_INFO("when_all await on thread={}", std::this_thread::get_id());
        auto [s1, s2] = co_await (std::move(c1) && std::move(c2));

        SPDLOG_INFO("All completed on thread={} with results {} {}",
                    std::this_thread::get_id(), s1, s2);
        co_return;
    };
    asio::co_spawn(ioc, launch(), asio::detached);
    ioc.run();
    pool.join();
}

} // namespace
