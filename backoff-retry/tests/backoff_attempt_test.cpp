#include <chrono>

#include "catch2/catch.hpp"

#include "backoff/backoff_attempt.h"

using namespace std::chrono_literals;

namespace {

TEST_CASE("Success without retry", "[backoff_attempt]") {
    constexpr auto delay = 4s;
    constexpr auto max_retries = 3u;
    int attempt = 0;
    auto result = backoff::attempt(backoff::make_constant(delay, max_retries), [&attempt] {
        ++attempt;
        return backoff::attempt_result::success;
    });

    CHECK(result.ok());
    CHECK(attempt == 1);
}

TEST_CASE("Success after retry", "[backoff_attempt]") {
    constexpr auto delay = 4s;
    constexpr auto max_retries = 3u;
    int attempt = 0;
    auto result = backoff::attempt(backoff::make_exponential(delay, max_retries), [&attempt] {
        return ++attempt == 4 ? backoff::attempt_result::success
                              : backoff::attempt_result::attempt_failure;
    });

    CHECK(result.ok());
    CHECK(attempt == 4);
}

TEST_CASE("Maximum retries exhausted", "[backoff_attempt]") {
    auto result = backoff::attempt(
        backoff::make_exponential_decorrelated_jitter(4s, 3, 15s),
        [] {
            return backoff::attempt_result::attempt_failure;
        });

    REQUIRE(!result.ok());
    CHECK(result.code == backoff::attempt_result::attempt_failure);
}

TEST_CASE("Abort due to hard error", "[backoff_attempt]") {
    auto result = backoff::attempt(
        backoff::make_exponential_decorrelated_jitter(4s, 3, 15s),
        [] {
            return backoff::attempt_result::hard_error;
        });

    REQUIRE(!result.ok());
    CHECK(result.code == backoff::attempt_result::hard_error);
}

} // namespace
