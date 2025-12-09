
#include <chrono>

#include <date/date.h>
#include <date/tz.h>
#include <fmt/chrono.h>
#include <fmt/format.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

TEST_CASE("custom time point with 20mins resolution") {
    constexpr intmax_t res = 1200;
    using twenty_mins_t = std::chrono::duration<int, std::ratio<res>>;
    auto tp = std::chrono::floor<twenty_mins_t>(std::chrono::system_clock::now());
    fmt::println("{:%Y-%m-%d %H:%M}", tp);
}

TEST_CASE("display datetime in local timezone") {
    const auto* my_timezone = std::chrono::current_zone();
    auto local_tp = my_timezone->to_local(std::chrono::system_clock::now());

    constexpr intmax_t res = 1200;
    using twenty_mins_t = std::chrono::duration<int, std::ratio<res>>;

    auto tp = std::chrono::floor<twenty_mins_t>(local_tp);
    fmt::println("{:%Y-%m-%d %H:%M}", tp);
}

// NOLINTBEGIN(readability-magic-numbers)

TEST_CASE("Convert year_month_day object to sys_days") {
    auto chloe_dob = date::year_month_day(date::year(2025), date::month(5), date::day(22));
    // The construction of year_month_day object may be invalid.
    REQUIRE(chloe_dob.ok());
    auto chloe_dob_tp = date::sys_days(chloe_dob);
    auto me_dob = date::year_month_day(date::year(1992), date::month(7), date::day(26));
    auto me_dob_tp = date::sys_days(me_dob);
    CHECK_EQ((chloe_dob_tp - me_dob_tp).count(), 11988);
}

TEST_CASE("Validness of year_month_day") {
    CHECK(date::year_month_day(date::year(2024), date::month(2), date::day(29)).ok());
    CHECK_FALSE(date::year_month_day(date::year(2025), date::month(2), date::day(30)).ok());
    CHECK_FALSE(date::year_month_day(date::year(2025), date::month(13), date::day(1)).ok());
}

TEST_CASE("The last day of the month") {
    SUBCASE("last day of feb in a leap year is 29") {
        auto eom = date::year_month_day_last(date::year(2024),
                                             date::month_day_last(date::month(2)));
        REQUIRE(eom.ok());
        CHECK_EQ(static_cast<unsigned>(eom.day()), 29);
    }

    SUBCASE("compose last day with more intuitive operator overload") {
        auto eom = date::year(2025) / date::May / date::last;
        REQUIRE(eom.ok());
        CHECK_EQ(static_cast<unsigned>(eom.day()), 31);
    }
}

TEST_CASE("Add to year_month_day") {
    SUBCASE("add years") {
        auto ymd = date::year(2024) / date::month(2) / date::day(1);
        REQUIRE(ymd.ok());
        ymd += date::years(1);
        CHECK(ymd.ok());
        CHECK_EQ(static_cast<int>(ymd.year()), 2025);
    }

    SUBCASE("add years but invalid") {
        auto ymd = date::year(2024) / date::month(2) / date::day(29);
        REQUIRE(ymd.ok());
        ymd += date::years(1);
        CHECK_FALSE(ymd.ok());
    }

    SUBCASE("add months") {
        auto ymd = date::year(2024) / date::month(2) / date::day(1);
        REQUIRE(ymd.ok());
        ymd += date::months(1);
        CHECK(ymd.ok());
        CHECK_EQ(static_cast<unsigned int>(ymd.month()), 3);
    }

    SUBCASE("add months but invalid") {
        auto ymd = date::year(2024) / date::month(1) / date::day(30);
        REQUIRE(ymd.ok());
        ymd += date::months(1);
        CHECK_FALSE(ymd.ok());
    }
}

TEST_CASE("Add days to year_month_day") {
    auto ymd = date::year(2024) / date::month(2) / date::day(29);
    REQUIRE(ymd.ok());
    ymd = date::sys_days(ymd) + date::days(2);
    CHECK_EQ(ymd, date::year(2024) / date::month(3) / date::day(2));
}

TEST_CASE("Specific datetime between time point") {
    using namespace std::chrono_literals;
    constexpr auto tp = std::chrono::sys_seconds{std::chrono::sys_days{2025y / 12 / 9} +
                                                 15h + 4min + 5s};
    fmt::println("{:%a, %d %b %Y %H:%M:%S %Z}", tp);

    constexpr auto year = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(tp))
                                  .year();
    static_assert(year == 2025y);

    constexpr auto time_part = std::chrono::hh_mm_ss<std::chrono::seconds>(
            tp - std::chrono::floor<std::chrono::days>(tp));
    static_assert(time_part.hours() == 15h);
    static_assert(time_part.minutes() == 4min);
    static_assert(time_part.seconds() == 5s);
}

TEST_CASE("Default inited and min sys_seconds") {
    static_assert(std::chrono::sys_seconds{}.time_since_epoch().count() == 0);
    static_assert(std::chrono::sys_seconds::min().time_since_epoch().count() < 0);
}

// NOLINTEND(readability-magic-numbers)
