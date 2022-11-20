//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "bloom-filter/bloom_filter.h"

#include "fmt/format.h"

TEST_CASE("throws exception when passed invalid arguments for construction") {
    CHECK_THROWS_AS({ bf::bloom_filter bf(0, 0.01); }, std::invalid_argument);
    CHECK_THROWS_AS({ bf::bloom_filter bf(100, 0); }, std::invalid_argument);
    CHECK_THROWS_AS({ bf::bloom_filter bf(100, 1); }, std::invalid_argument);
    CHECK_THROWS_AS({ bf::bloom_filter bf(100, 10); }, std::invalid_argument);
}

TEST_CASE("adjust optimal max entries with given error rate") {
    SUBCASE("with error rate 0.1%") {
        constexpr double error_rate = 0.001;
        bf::bloom_filter bf(1000, error_rate);
        CHECK(bf.max_entries() >= 1000);
    }

    SUBCASE("with error rate 1%") {
        constexpr double error_rate = 0.01;
        bf::bloom_filter bf(1000, error_rate);
        CHECK(bf.max_entries() >= 1000);
    }
}

TEST_CASE("add and contain") {
    constexpr double error_rate = 0.01;
    bf::bloom_filter bf(1000, error_rate);
    std::string data_list[] = {"Alfa",
                               "Bravo",
                               "Charlie",
                               "Delta",
                               "Echo"};
    SUBCASE("initially not present") {
        for (const auto& s : data_list) {
            INFO("str=", s);
            CHECK_FALSE(bf.may_contain(s.data(), static_cast<int>(s.size())));
        }
    }

    SUBCASE("should contain when added") {
        for (const auto& s : data_list) {
            INFO("str=", s);
            REQUIRE(bf.add(s.data(), static_cast<int>(s.size())));
        }

        for (const auto& s : data_list) {
            INFO("str=", s);
            CHECK(bf.may_contain(s.data(), static_cast<int>(s.size())));
        }
    }
}

int main(int argc, const char* argv[]) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
