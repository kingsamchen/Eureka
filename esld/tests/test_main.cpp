//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include <string_view>
#include <vector>

#include "esld/psl.h"

#include "esl/strings.h"

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

int main(int argc, const char* argv[]) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

 TEST_CASE("read from file and parse into data") {
     std::ignore = psl::from_file(R"(F:\projects\Eureka\esld\public_suffix_list.dat)");
 }

TEST_CASE("parse from data and sort rules") {
    SUBCASE("list from github repo") {
        const std::string unprocessed[]{"beta.example.net"s,
                                        "alpha.beta.example.com"s,
                                        "delta.example.net"s,
                                        "alpha.example.test"s,
                                        "charlie.example.org"s,
                                        "beta.example.com"s,
                                        "delta.example.invalid"s,
                                        "buzz"s,
                                        "zz"s};
        auto finalized = esl::strings::split(psl::from_data(esl::strings::join(unprocessed, "\n")),
                                             psl::rule_delim)
                                 .to<std::vector<std::string>>();

        const std::vector sorted{"buzz"s,
                                 "beta.example.com"s,
                                 "alpha.beta.example.com"s,
                                 "delta.example.invalid"s,
                                 "beta.example.net"s,
                                 "delta.example.net"s,
                                 "charlie.example.org"s,
                                 "alpha.example.test"s,
                                 "zz"s};
        CHECK_EQ(finalized, sorted);
    }

    SUBCASE("list with exception rules and wildcard rules") {
        const std::string unprocessed[]{"*.kawasaki.jp"s,
                                        "*.kitakyushu.jp"s,
                                        "!city.kawasaki.jp"s,
                                        "!city.kitakyushu.jp"s,
                                        "kasugai.aichi.jp"s,
                                        "jp"s,
                                        "*.customer-oci.com"s,
                                        "*.oci.customer-oci.com"s,
                                        "*.ocp.customer-oci.com"s,
                                        "*.ocs.customer-oci.com"s};
        auto finalized = esl::strings::split(psl::from_data(esl::strings::join(unprocessed, "\n")),
                                             psl::rule_delim)
                                 .to<std::vector<std::string>>();
        const std::vector sorted{"*.customer-oci.com"s,
                                 "*.oci.customer-oci.com"s,
                                 "*.ocp.customer-oci.com"s,
                                 "*.ocs.customer-oci.com"s,
                                 "jp"s,
                                 "kasugai.aichi.jp"s,
                                 "!city.kawasaki.jp"s,
                                 "*.kawasaki.jp"s,
                                 "!city.kitakyushu.jp"s,
                                 "*.kitakyushu.jp"s};
        CHECK_EQ(finalized, sorted);
    }
}

TEST_CASE("match") {
    const std::string unprocessed[]{"*.kawasaki.jp"s,
                                    "*.kitakyushu.jp"s,
                                    "!city.kawasaki.jp"s,
                                    "!city.kitakyushu.jp"s,
                                    "kasugai.aichi.jp"s,
                                    "aichi.jp"s,
                                    "jp"s,
                                    "*.customer-oci.com"s,
                                    "*.oci.customer-oci.com"s,
                                    "*.ocp.customer-oci.com"s,
                                    "*.ocs.customer-oci.com"s};
    std::string rule_data = psl::from_data(esl::strings::join(unprocessed, "\n"));

    SUBCASE("most top-level domain") {
        CHECK_EQ(psl::match_tld(rule_data, "test.jp"), "jp");
        CHECK_EQ(psl::match_tld(rule_data, "jp"), "jp");
    }

    SUBCASE("normal rule") {
        CHECK_EQ(psl::match_tld(rule_data, "foobar.aichi.jp"), "aichi.jp");
        CHECK_EQ(psl::match_tld(rule_data, "foobar.kasugai.aichi.jp"), "kasugai.aichi.jp");
        CHECK_EQ(psl::match_tld(rule_data, "foobar.aichi.kasugai.jp"), "jp");
    }

    SUBCASE("wildcard rule") {
        CHECK_EQ(psl::match_tld(rule_data, "foobar.kawasaki.jp"), "foobar.kawasaki.jp");
        CHECK_EQ(psl::match_tld(rule_data, "test.foobar.kawasaki.jp"), "foobar.kawasaki.jp");
        CHECK_EQ(psl::match_tld(rule_data, "kawasaki.jp"), "kawasaki.jp");
    }

    SUBCASE("exception rule") {
        CHECK_EQ(psl::match_tld(rule_data, "city.kawasaki.jp"), "kawasaki.jp");
        CHECK_EQ(psl::match_tld(rule_data, "abc.city.kawasaki.jp"), "kawasaki.jp");
    }
}

TEST_CASE("complete data") {
    auto rule_data = psl::from_file(R"(F:\projects\Eureka\esld\public_suffix_list.dat)");

    CHECK_EQ(psl::match_tld(rule_data, "example.com"), "com");
    CHECK_EQ(psl::match_tld(rule_data, "example.co.za"), "co.za");
    CHECK_EQ(psl::match_tld(rule_data, "example.in.net"), "in.net");
    CHECK_EQ(psl::match_tld(rule_data, "example.star.kawasaki.jp"), "star.kawasaki.jp");
    CHECK_EQ(psl::match_tld(rule_data, "example.co.uk"), "co.uk");
    CHECK_EQ(psl::match_tld(rule_data, "test.dev.example.co.uk"), "co.uk");
}
