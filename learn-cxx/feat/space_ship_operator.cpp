#include <compare>
#include <string>
#include <type_traits>
#include <utility>

#include <doctest/doctest.h>
#include <fmt/format.h>

namespace {

TEST_CASE("Normal usage") {
    struct mixed {
        int n;
        double d;
        std::string s;

        auto operator<=>(const mixed&) const = default;
    };

    static_assert(std::is_same_v<std::compare_three_way_result_t<mixed>,
                                 std::partial_ordering>);

    static_assert(std::is_same_v<decltype(std::declval<const mixed&>() <=> std::declval<const mixed&>()),
                                 std::partial_ordering>);

    // `std::invoke_result_t<>` expects a callable tpye, cannot pass mixed::operator<=>
    //  directly, without casting it to a function pointer.
    constexpr auto spaceship = [](const auto& a, const auto& b) {
        return a <=> b;
    };
    static_assert(std::is_same_v<std::invoke_result_t<decltype(spaceship),
                                                      const mixed&, const mixed&>,
                                 std::partial_ordering>);

    mixed m1{.n = 42, .d = 1., .s = "hello"};
    mixed m2{.n = 64, .d = 0., .s = "HELLO"};

    CHECK_FALSE(m1 == m2);
    CHECK(m1 != m2);

    CHECK(m1 < m2);
    CHECK(m1 <= m2);

    CHECK_FALSE(m1 > m2);
}

TEST_CASE("Manual Impl 3-way") {
    struct mixed {
        int n;
        double d;
        std::string s;

        // Ignore `n`
        auto operator<=>(const mixed& other) const {
            return std::tie(n, s) <=> std::tie(other.n, other.s);
        }
    };

    static_assert(std::is_same_v<std::compare_three_way_result_t<mixed>,
                                 std::strong_ordering>);

    mixed m1{.n = 42, .d = 1., .s = "hello"};
    mixed m2{.n = 42, .d = 0., .s = "hello"};

    CHECK(m1 <= m2);
    CHECK(m1 >= m2);

    // Non-defaulted operator<=> would prohibit compiler synthesizing defaulted operator==
    // CHECK(m1 == m2);
}

TEST_CASE("Rewrites skip user-defined") {
    struct F {
        int x;

        std::strong_ordering operator<=>(const F& other) const {
            fmt::println("{}", "<=> called");
            return x <=> other.x;
        }

        bool operator<(const F& other) const {
            fmt::println("{}", "< called");
            return x < other.x;
        }
    };

    F a{1};
    F b{2};
    [[maybe_unused]] bool r1 = a < b;  // Prints "< called"  — your operator< wins
    [[maybe_unused]] bool r2 = a > b;  // Prints "<=> called" — synthesized from <=>
    [[maybe_unused]] bool r3 = a <= b; // Prints "<=> called" — synthesized from <=>
}

TEST_CASE("Reverse rewrite candidates") {
    struct Number {
        int value;

        // Suppress implicit conversion
        explicit Number(int v) : value(v) {}

        auto operator<=>(int n) const {
            return value <=> n;
        }

        bool operator==(int n) const {
            return value == n;
        }
    };

    Number n{42};
    CHECK(1 < n);
    CHECK_FALSE(1 > n);
    CHECK(42 == n);
}

} // namespace
