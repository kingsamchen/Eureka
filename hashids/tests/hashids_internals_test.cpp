//
// Kingsley Chen
//

#include <string>
#include <string_view>

#include "doctest/doctest.h"

namespace xx {

std::string make_alphabet_salt(char lottery, const std::string& salt, const std::string& alphabet);

} // namespace xx

namespace {

std::string concat(char l, std::string_view salt, std::string_view abt) {
    std::string s;
    s += l;
    s += salt;
    s += abt;
    return s.substr(0, abt.size());
}

TEST_SUITE_BEGIN("hashids/internals");

TEST_CASE("make alphabet salt") {
    constexpr char lottery = 'k';
    constexpr std::string_view test_alphabet = "abcdefghijklmnopqrstuvwxyz";
    constexpr std::string_view test_salt = "this is a salt";

    SUBCASE("the usual case: use lottery/salt/alphabet") {
        const auto s1 = xx::make_alphabet_salt(lottery,
                                               std::string{test_salt},
                                               std::string{test_alphabet});
        const auto s2 = concat(lottery, test_salt, test_alphabet);
        CHECK_EQ(s1, s2);
    }

    SUBCASE("empty salt") {
        const std::string empty_salt;
        auto s1 = xx::make_alphabet_salt(lottery, empty_salt, std::string{test_alphabet});
        auto s2 = concat(lottery, empty_salt, test_alphabet);
        CHECK_EQ(s1, s2);
    }

    SUBCASE("salt is longer than alphabet") {
        const std::string long_salt{test_alphabet};
        const std::string short_alphabet = long_salt.substr(long_salt.size() / 2);
        REQUIRE(long_salt.size() > short_alphabet.size());
        auto s1 = xx::make_alphabet_salt(lottery, long_salt, short_alphabet);
        auto s2 = concat(lottery, long_salt, short_alphabet);
        CHECK_EQ(s1, s2);
    }
}

TEST_SUITE_END();

} // namespace
