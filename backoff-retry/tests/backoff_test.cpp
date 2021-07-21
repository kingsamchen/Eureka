#include <chrono>
#include <string>

#include "catch2/catch.hpp"

#include "backoff/backoff.h"
#include "backoff/backoff_policy.h"

using constant_backoff = backoff::backoff<backoff::constant_policy>;
using linear_backoff = backoff::backoff<backoff::linear_policy>;

using namespace std::chrono_literals;

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
    CHECK(base_size + sizeof(backoff::linear_policy) == sizeof(linear_backoff));
}

} // namespace
