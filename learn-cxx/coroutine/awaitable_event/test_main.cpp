// Kingsley Chen

#include <atomic>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "awaitable_event/event.h"
#include "awaitable_event/noreturn.h"

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

// Steal from https://github.com/luncliff/coroutine/blob/ad1e682fe9a01a250e71080366d653af60eda708/test/c2_concrt_event.cpp
namespace {

// NOLINTNEXTLINE(cppcoreguidelines-avoid-reference-coroutine-parameters)
coro::no_return wait_for_one_event(coro::waitable_event& e, std::atomic_flag& flag) {
    try {
        // resume after the event is signaled ...
        co_await e;
    } catch (std::system_error& ex) {
        // event throws if there was an internal system error
        FAIL(ex.what());
    }
    flag.test_and_set();
}

} // namespace

TEST_CASE("no event wait yields no task") {
    auto count = 0;
    for (auto task : coro::signaled_events()) {
        task.resume();
        ++count;
    }
    REQUIRE(count == 0);
}

TEST_CASE("wait for one event") {
    coro::waitable_event e{};
    std::atomic_flag flag{};

    wait_for_one_event(e, flag);
    e.signal();
    auto count = 0;

    for (auto task : coro::signaled_events()) {
        task.resume();
        ++count;
    }
    REQUIRE(count > 0);
    // already set by the coroutine `wait_for_one_event`
    REQUIRE(flag.test_and_set() == true);
}
