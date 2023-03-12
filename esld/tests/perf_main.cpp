//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include <string_view>
#include <vector>

#include "esl/strings.h"
#include "esld/psl.h"
#include "fmt/format.h"

#define CHECK_EQ(lhs, rhs)                                                                                \
    do {                                                                                                  \
        lhs == rhs ? fmt::print("OK\n") : fmt::print(#lhs " <-> " #rhs "\n\tlhs={}\n\trhs={}", lhs, rhs); \
    } while (false)

void run_test(const std::string& path) {
    auto rule_data = psl::from_file(path);

    CHECK_EQ(psl::match_tld(rule_data, "example.com"), "com");
    CHECK_EQ(psl::match_tld(rule_data, "example.co.za"), "co.za");
    CHECK_EQ(psl::match_tld(rule_data, "example.in.net"), "in.net");
    CHECK_EQ(psl::match_tld(rule_data, "example.star.kawasaki.jp"), "star.kawasaki.jp");
    CHECK_EQ(psl::match_tld(rule_data, "example.co.uk"), "co.uk");
    CHECK_EQ(psl::match_tld(rule_data, "test.dev.example.co.uk"), "co.uk");
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fmt::print(stderr, "must specifcy public_suffix_list.dat path!");
        return 1;
    }

    run_test(argv[1]);

    return 0;
}
