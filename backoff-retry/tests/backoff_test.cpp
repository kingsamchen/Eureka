#include <chrono>
#include <string>

#include "catch2/catch.hpp"

#include "backoff/backoff.h"
#include "backoff/backoff_policy.h"

using constant_backoff = backoff::backoff<backoff::constant_policy>;
using linear_backoff = backoff::backoff<backoff::linear_policy>;

using namespace std::chrono_literals;

template<typename T>
void mark_unused(T&&) {}

namespace {

TEST_CASE("Maximum retries reached", "[backoff]") {
    constexpr uint32_t max_retries = 5;
    constant_backoff backoff(3s, max_retries);
    for (auto i = 0u; i < max_retries; ++i) {
        auto delay = backoff.next_delay();
        CHECK(delay.has_value());
    }

    auto delay = backoff.next_delay();
    CHECK(!delay.has_value());
    delay = backoff.next_delay();
    CHECK(!delay.has_value());
}

TEST_CASE("Reset of backoff", "[backoff]") {
    constexpr uint32_t max_retries = 3;
    constant_backoff backoff(3s, max_retries);
    for (auto i = 0u; i < max_retries; ++i) {
        auto delay = backoff.next_delay();
        CHECK(delay.has_value());
    }

    REQUIRE(!backoff.next_delay().has_value());

    backoff.reset();
    for (auto i = 0u; i < max_retries; ++i) {
        auto delay = backoff.next_delay();
        CHECK(delay.has_value());
    }

    CHECK(!backoff.next_delay().has_value());
}

TEST_CASE("Enable jitter", "[backoff]") {
    constexpr auto initial_delay = 4s;
    constexpr auto max_retries = 5u;
    backoff::exponential_policy::options opt(30s);
    backoff::backoff ebf(initial_delay,
                         max_retries,
                         backoff::exponential_policy(opt),
                         backoff::full_jitter(4s));
    for (auto i = 0u; i < max_retries; ++i) {
        auto delay = ebf.next_delay();
        REQUIRE(delay.has_value());
        CHECK((4s <= *delay && *delay <= 30s));
    }
}

TEST_CASE("Optimized by empty classes", "[backoff]") {
    constexpr size_t base_size = sizeof(backoff::duration_type) + sizeof(uint32_t) * 2;
    CHECK(base_size == sizeof(constant_backoff));
    CHECK(base_size + sizeof(backoff::linear_policy) == sizeof(linear_backoff));
}

TEST_CASE("Make constants", "[make_backoff]") {
    auto cb = backoff::make_constant(4s, 3);
    mark_unused(cb);

    auto cb_with_jitter = backoff::make_constant<backoff::full_jitter>(
        4s, 3, backoff::full_jitter{});
    mark_unused(cb_with_jitter);
}

TEST_CASE("Make linears", "[make_backoff]") {
    auto lb = backoff::make_linear(4s, 3, 5s);
    mark_unused(lb);

    auto lb_with_jitter = backoff::make_linear<backoff::full_jitter>(
        4s, 3, backoff::linear_policy::options(5s, 60s), backoff::full_jitter{});
    mark_unused(lb_with_jitter);
}

} // namespace
