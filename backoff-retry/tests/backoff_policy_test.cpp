#include <algorithm>
#include <chrono>

#include "catch2/catch.hpp"

#include "backoff/backoff_policy.h"

using namespace std::chrono_literals;

namespace {

TEST_CASE("Constant backoff policy", "[policy]") {
    SECTION("delay is constant") {
        backoff::constant_policy policy;
        constexpr auto base_delay = 1s;
        constexpr auto max_retries = 10u;
        for (auto i = 0u; i < max_retries; ++i) {
            CHECK(policy.apply(base_delay, i) == base_delay);
        }
    }
}

TEST_CASE("Linear backoff policy", "[policy]") {
    SECTION("delay is a arithmetic sequence") {
        auto increment = 5s;
        backoff::linear_policy policy(backoff::linear_policy::options{increment});

        constexpr auto base_delay = 10s;
        constexpr auto max_retries = 5u;

        std::vector<std::chrono::milliseconds> delay_seq;
        std::generate_n(std::back_inserter(delay_seq), max_retries, [&]() {
            return policy.apply(base_delay, static_cast<uint32_t>(delay_seq.size()));
        });

        std::vector<std::chrono::milliseconds> expected_seq{10s, 15s, 20s, 25s, 30s};

        CHECK(delay_seq == expected_seq);
    }

    SECTION("maximum delay") {
        auto increment = 15s;
        auto max_delay = 50s;
        backoff::linear_policy policy({increment, max_delay});

        constexpr auto base_delay = 10s;
        constexpr auto max_retries = std::numeric_limits<uint32_t>::max();
        std::vector<std::chrono::milliseconds> delay_seq;
        std::generate_n(std::back_inserter(delay_seq), 7, [&]() {
            return policy.apply(base_delay, static_cast<uint32_t>(delay_seq.size()));
        });

        std::vector<std::chrono::milliseconds> expected_seq{10s, 25s, 40s, 50s, 50s, 50s, 50s};
        CHECK(delay_seq == expected_seq);
    }
}

TEST_CASE("Exponential backoff policy", "[policy]") {
    SECTION("delay is a geometric sequence") {
        constexpr auto base_delay = 4s;
        constexpr auto max_retries = 5u;
        backoff::exponential_policy policy;

        std::vector<std::chrono::milliseconds> delay_seq;
        std::generate_n(std::back_inserter(delay_seq), max_retries, [&]() {
            return policy.apply(base_delay, static_cast<uint32_t>(delay_seq.size()));
        });

        // max delay cut-off.
        std::vector<std::chrono::milliseconds> expected_seq{4s, 8s, 16s, 32s, 64s};

        CHECK(delay_seq == expected_seq);
    }

    SECTION("maximum delay") {
        auto opt = backoff::exponential_policy::options(60s);
        backoff::exponential_policy policy(opt);

        constexpr auto base_delay = 8s;
        constexpr auto max_retries = 5u;

        std::vector<std::chrono::milliseconds> delay_seq;
        std::generate_n(std::back_inserter(delay_seq), max_retries, [&]() {
            return policy.apply(base_delay, static_cast<uint32_t>(delay_seq.size()));
        });

        // max delay cut-off.
        std::vector<std::chrono::milliseconds> expected_seq{8s, 16s, 32s, 60s, 60s};

        CHECK(delay_seq == expected_seq);
    }
}

} // namespace
