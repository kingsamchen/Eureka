#include <chrono>
#include <string>

#include "catch2/catch.hpp"

#include "backoff/backoff.h"
#include "backoff/backoff_policy.h"

using constant_backoff = backoff::backoff<backoff::constant>;
using linear_backoff = backoff::backoff<backoff::linear>;

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

TEST_CASE("Optimized by empty classes", "[backoff]") {
    constexpr size_t base_size = sizeof(backoff::duration_type) + sizeof(uint32_t) * 2;
    CHECK(base_size == sizeof(constant_backoff));
    CHECK(base_size + sizeof(backoff::linear) == sizeof(linear_backoff));
}

TEST_CASE("Make backoff with policies", "[make_backoff]") {
    SECTION("make_constant") {
        auto cb = backoff::make_constant(4s, 3);
        mark_unused(cb);
    }

    SECTION("make_linear") {
        auto lb = backoff::make_linear(4s, 3, 5s);
        mark_unused(lb);

        auto lb_max_delay = backoff::make_linear(4s, 5, 5s, 30s);
        mark_unused(lb_max_delay);
    }

    SECTION("make_exponential") {
        auto eb = backoff::make_exponential(4s, 5);
        mark_unused(eb);

        auto eb_max_delay = backoff::make_exponential(4s, 5, 60s);
        mark_unused(eb_max_delay);
    }

    SECTION("make_exponential_full_jitter") {
        auto efj = backoff::make_exponential_full_jitter(4s, 5);
        mark_unused(efj);

        auto efj_max_delay = backoff::make_exponential_full_jitter(4s, 5, 60s);
        mark_unused(efj_max_delay);
    }

    SECTION("make_exponential_decorrelated_jitter") {
        auto edj = backoff::make_exponential_decorrelated_jitter(4s, 5);
        mark_unused(edj);

        auto edj_max_delay = backoff::make_exponential_decorrelated_jitter(4s, 5, 60s);
        mark_unused(edj_max_delay);
    }
}

} // namespace
