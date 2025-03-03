
#include <chrono>

#include "fmt/chrono.h"
#include "fmt/format.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

TEST_CASE("custom time point with 20mins resolution") {
    constexpr intmax_t res = 1200;
    using twenty_mins_t = std::chrono::duration<int, std::ratio<res>>;
    auto tp = std::chrono::floor<twenty_mins_t>(std::chrono::system_clock::now());
    fmt::println("{:%Y-%m-%d %H:%M}\n", tp);
}

#if __cpp_lib_chrono >= 201907L

TEST_CASE("display datetime in local timezone") {
    auto my_timezone = std::chrono::current_zone();
    auto local_tp = my_timezone->to_local(std::chrono::system_clock::now());

    constexpr intmax_t res = 1200;
    using twenty_mins_t = std::chrono::duration<int, std::ratio<res>>;

    auto tp = std::chrono::floor<twenty_mins_t>(local_tp);
    fmt::println("{:%Y-%m-%d %H:%M}\n", tp);
}

#endif
