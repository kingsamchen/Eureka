#include <chrono>

#include "catch2/catch.hpp"

#include "backoff/backoff.h"

using constant_backoff = backoff::backoff<backoff::constant_policy>;
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
    CHECK_FALSE(delay.has_value());
    delay = backoff.next_delay();
    CHECK_FALSE(delay.has_value());
}

} // namespace
