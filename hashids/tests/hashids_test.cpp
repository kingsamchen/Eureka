//
// Kingsley Chen
//

#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "doctest/doctest.h"
#include "fmt/format.h"
#include "fmt/ranges.h"

#include "hashids/hashids.h"

namespace {

TEST_SUITE_BEGIN("hashids");

// NOLINTBEGIN(readability-magic-numbers)

TEST_CASE("with default options") {
    const xx::hashids hashids;

    SUBCASE("encode single number") {
        const std::tuple<std::uint64_t, std::string> cases[]{
                {12345, "j0gW"},
                {0, "gY"},
                {1, "jR"},
                {22, "Lw"},
                {333, "Z0E"},
                {9999, "w0rR"},
        };

        for (const auto& [number, hash] : cases) {
            CHECK_EQ(hashids.encode(&number, 1), hash);
        }
    }

    SUBCASE("encode multiple numbers") {
        const std::tuple<std::vector<std::uint64_t>, std::string> cases[]{
                {{1, 2, 3}, "o2fXhV"},
                {{2, 4, 6}, "xGhmsW"},
                {{99, 25}, "3lKfD"},
                {{683, 94108, 123, 5}, "vJvi7On9cXGtD"},
        };

        for (const auto& [numbers, hash] : cases) {
            auto num_str = fmt::to_string(numbers);
            INFO("numbers=", num_str);
            CHECK_EQ(hashids.encode(numbers.data(), numbers.size()), hash);
        }
    }

    SUBCASE("decode for single number") {
        const std::tuple<std::string, std::uint64_t> cases[]{
                {"j0gW", 12345},
                {"gY", 0},
                {"jR", 1},
                {"Lw", 22},
                {"Z0E", 333},
                {"w0rR", 9999},
        };

        for (const auto& [hash, number] : cases) {
            CHECK_EQ(hashids.decode(hash)[0], number);
        }
    }

    SUBCASE("decode for multiple numbers") {
        const std::tuple<std::string, std::vector<std::uint64_t>> cases[]{
                {"o2fXhV", {1, 2, 3}},
                {"xGhmsW", {2, 4, 6}},
                {"3lKfD", {99, 25}},
                {"vJvi7On9cXGtD", {683, 94108, 123, 5}},
        };

        for (const auto& [hash, numbers] : cases) {
            CHECK_EQ(hashids.decode(hash), numbers);
        }
    }
}

TEST_CASE("with salt") {
    xx::hashids::options opts;
    opts.salt = "Arbitrary string";
    const xx::hashids hashids(opts);

    SUBCASE("encode") {
        const std::tuple<std::vector<std::uint64_t>, std::string> cases[]{
                {{683, 94108, 123, 5}, "QWyf8yboH7KT2"},
                {{1, 2, 3}, "neHrCa"},
                {{2, 4, 6}, "LRCgf2"},
                {{99, 25}, "JOMh1"},
        };

        for (const auto& [numbers, hash] : cases) {
            auto num_str = fmt::to_string(numbers);
            INFO("numbers=", num_str);
            CHECK_EQ(hashids.encode(numbers.data(), numbers.size()), hash);
        }
    }

    SUBCASE("decode") {
        const std::tuple<std::string, std::vector<std::uint64_t>> cases[]{
                {"QWyf8yboH7KT2", {683, 94108, 123, 5}},
                {"neHrCa", {1, 2, 3}},
                {"LRCgf2", {2, 4, 6}},
                {"JOMh1", {99, 25}},
        };

        for (const auto& [hash, numbers] : cases) {
            CHECK_EQ(hashids.decode(hash), numbers);
        }
    }
}

TEST_CASE("custom alphabet") {
    xx::hashids::options opts;
    opts.custom_alphabet =
            "!\"#%&',-/0123456789:;<=>ABCDEFGHIJKLMNOPQRSTUVWXYZ_`abcdefghijklmnopqrstuvwxyz~";
    const xx::hashids hashids(opts);

    SUBCASE("encode") {
        const std::tuple<std::vector<std::uint64_t>, std::string> cases[]{
                {{2839, 12, 32, 5}, "_nJUNTVU3"},
                {{1, 2, 3}, "7xfYh2"},
                {{23832}, "Z6R>"},
                {{99, 25}, "AYyIB"},
        };

        for (const auto& [numbers, hash] : cases) {
            auto num_str = fmt::to_string(numbers);
            INFO("numbers=", num_str);
            CHECK_EQ(hashids.encode(numbers.data(), numbers.size()), hash);
        }
    }

    SUBCASE("decode") {
        const std::tuple<std::string, std::vector<std::uint64_t>> cases[]{
                {"_nJUNTVU3", {2839, 12, 32, 5}},
                {"7xfYh2", {1, 2, 3}},
                {"Z6R>", {23832}},
                {"AYyIB", {99, 25}},
        };

        for (const auto& [hash, numbers] : cases) {
            CHECK_EQ(hashids.decode(hash), numbers);
        }
    }
}

TEST_CASE("short alphabet") {
    xx::hashids::options opts;
    opts.custom_alphabet = "ABcfhistuCFHISTU";
    const xx::hashids hashids(opts);

    SUBCASE("encode") {
        const std::tuple<std::vector<std::uint64_t>, std::string> cases[]{
                {{2839, 12, 32, 5}, "AABAABBBABAAAuBBAAUABBBBBCBAB"},
                {{1, 2, 3}, "AAhBAiAA"},
                {{23832}, "AABAAABABBBAABBB"},
                {{99, 25}, "AAABBBAAHBBAAB"},
        };

        for (const auto& [numbers, hash] : cases) {
            auto num_str = fmt::to_string(numbers);
            INFO("numbers=", num_str);
            CHECK_EQ(hashids.encode(numbers.data(), numbers.size()), hash);
        }
    }

    SUBCASE("decode") {
        const std::tuple<std::string, std::vector<std::uint64_t>> cases[]{
                {"AABAABBBABAAAuBBAAUABBBBBCBAB", {2839, 12, 32, 5}},
                {"AAhBAiAA", {1, 2, 3}},
                {"AABAAABABBBAABBB", {23832}},
                {"AAABBBAAHBBAAB", {99, 25}},
        };

        for (const auto& [hash, numbers] : cases) {
            CHECK_EQ(hashids.decode(hash), numbers);
        }
    }
}

TEST_CASE("minimum length") {
    xx::hashids::options opts;
    opts.minimum_length = 25;
    const xx::hashids hashids(opts);

    SUBCASE("encode") {
        const std::tuple<std::vector<std::uint64_t>, std::string> cases[]{
                {{7452, 2967, 21401}, "pO3K69b86jzc6krI416enr2B5"},
                {{1, 2, 3}, "gyOwl4B97bo2fXhVaDR0Znjrq"},
                {{6097}, "Nz7x3VXyMYerRmWeOBQn6LlRG"},
                {{99, 25}, "k91nqP3RBe3lKfDaLJrvy8XjV"},
        };

        for (const auto& [numbers, hash] : cases) {
            auto num_str = fmt::to_string(numbers);
            INFO("numbers=", num_str);
            CHECK_EQ(hashids.encode(numbers.data(), numbers.size()), hash);
        }
    }

    SUBCASE("decode") {
        const std::tuple<std::string, std::vector<std::uint64_t>> cases[]{
                {"pO3K69b86jzc6krI416enr2B5", {7452, 2967, 21401}},
                {"gyOwl4B97bo2fXhVaDR0Znjrq", {1, 2, 3}},
                {"Nz7x3VXyMYerRmWeOBQn6LlRG", {6097}},
                {"k91nqP3RBe3lKfDaLJrvy8XjV", {99, 25}},
        };

        for (const auto& [hash, numbers] : cases) {
            CHECK_EQ(hashids.decode(hash), numbers);
        }
    }
}

TEST_CASE("all options") {
    xx::hashids::options opts;
    opts.salt = "arbitrary salt";
    opts.minimum_length = 16;
    opts.custom_alphabet = "abcdefghijklmnopqrstuvwxyz";
    const xx::hashids hashids(opts);

    SUBCASE("encode") {
        const std::tuple<std::vector<std::uint64_t>, std::string> cases[]{
                {{7452, 2967, 21401}, "wygqxeunkatjgkrw"},
                {{1, 2, 3}, "pnovxlaxuriowydb"},
                {{60125}, "jkbgxljrjxmlaonp"},
                {{99, 25}, "erdjpwrgouoxlvbx"},
        };

        for (const auto& [numbers, hash] : cases) {
            auto num_str = fmt::to_string(numbers);
            INFO("numbers=", num_str);
            CHECK_EQ(hashids.encode(numbers.data(), numbers.size()), hash);
        }
    }

    SUBCASE("decode") {
        const std::tuple<std::string, std::vector<std::uint64_t>> cases[]{
                {"wygqxeunkatjgkrw", {7452, 2967, 21401}},
                {"pnovxlaxuriowydb", {1, 2, 3}},
                {"jkbgxljrjxmlaonp", {60125}},
                {"erdjpwrgouoxlvbx", {99, 25}},
        };

        for (const auto& [hash, numbers] : cases) {
            CHECK_EQ(hashids.decode(hash), numbers);
        }
    }
}

TEST_CASE("alphabet without standard separator") {
    xx::hashids::options opts;
    opts.custom_alphabet = "abdegjklmnopqrvwxyzABDEGJKLMNOPQRVWXYZ1234567890";
    const xx::hashids hashids(opts);

    SUBCASE("encode") {
        const std::tuple<std::vector<std::uint64_t>, std::string> cases[]{
                {{7452, 2967, 21401}, "X50Yg6VPoAO4"},
                {{1, 2, 3}, "GAbDdR"},
                {{60125}, "5NMPD"},
                {{99, 25}, "yGya5"},
        };

        for (const auto& [numbers, hash] : cases) {
            auto num_str = fmt::to_string(numbers);
            INFO("numbers=", num_str);
            CHECK_EQ(hashids.encode(numbers.data(), numbers.size()), hash);
        }
    }

    SUBCASE("decode") {
        const std::tuple<std::string, std::vector<std::uint64_t>> cases[]{
                {"X50Yg6VPoAO4", {7452, 2967, 21401}},
                {"GAbDdR", {1, 2, 3}},
                {"5NMPD", {60125}},
                {"yGya5", {99, 25}},
        };

        for (const auto& [hash, numbers] : cases) {
            CHECK_EQ(hashids.decode(hash), numbers);
        }
    }
}

TEST_CASE("alphabet with two standard separators") {
    xx::hashids::options opts;
    opts.custom_alphabet = "abdegjklmnopqrvwxyzABDEGJKLMNOPQRVWXYZ1234567890uC";
    const xx::hashids hashids(opts);

    SUBCASE("encode") {
        const std::tuple<std::vector<std::uint64_t>, std::string> cases[]{
                {{7452, 2967, 21401}, "GJNNmKYzbPBw"},
                {{1, 2, 3}, "DQCXa4"},
                {{60125}, "38V1D"},
                {{99, 25}, "373az"},
        };

        for (const auto& [numbers, hash] : cases) {
            auto num_str = fmt::to_string(numbers);
            INFO("numbers=", num_str);
            CHECK_EQ(hashids.encode(numbers.data(), numbers.size()), hash);
        }
    }

    SUBCASE("decode") {
        const std::tuple<std::string, std::vector<std::uint64_t>> cases[]{
                {"GJNNmKYzbPBw", {7452, 2967, 21401}},
                {"DQCXa4", {1, 2, 3}},
                {"38V1D", {60125}},
                {"373az", {99, 25}},
        };

        for (const auto& [hash, numbers] : cases) {
            CHECK_EQ(hashids.decode(hash), numbers);
        }
    }
}

TEST_CASE("decode empty hashid") {
    const xx::hashids hashids;
    auto nums = hashids.decode("");
    CHECK(nums.empty());
}

TEST_CASE("decode invalid hashid") {
    xx::hashids::options opts;
    opts.custom_alphabet = "abcdefghijklmnop";
    const xx::hashids hashids(opts);
    CHECK_THROWS(hashids.decode("qrstuvwxyz"));
}

TEST_CASE("decode only one valid") {
    xx::hashids::options opts;
    opts.minimum_length = 6;
    const xx::hashids hashids(opts);
    const std::uint64_t num{1U};
    auto hashed = hashids.encode(&num, 1);
    hashed.back() = '0';
    CHECK_THROWS(hashids.decode(hashed));
}

TEST_CASE("encode hex") {
    const xx::hashids hashids;
    CHECK_EQ(hashids.encode_hex("507f1f77bcf86cd799439011"), "y42LW46J9luq3Xq9XMly");
    CHECK_EQ(hashids.encode_hex("f000000000000000000000000000000000000000000000000000000000000000000000000000000000000f"),
             "WxMLpERDrmh25Lp4L3xEfM6WovWYO3IjkRMKR2ogCMVzn4zQlqt1WK8jKq7OsEpy2qyw1Vi2p");

    SUBCASE("empty hex") {
        CHECK_EQ(hashids.encode_hex(""), "");
    }

    SUBCASE("illegal") {
        CHECK_THROWS(hashids.encode_hex("1234SGT8"));
    }
}

TEST_CASE("decode hex") {
    const xx::hashids hashids;
    CHECK_EQ(hashids.decode_hex("y42LW46J9luq3Xq9XMly"), "507f1f77bcf86cd799439011");
    CHECK_EQ(hashids.decode_hex("WxMLpERDrmh25Lp4L3xEfM6WovWYO3IjkRMKR2ogCMVzn4zQlqt1WK8jKq7OsEpy2qyw1Vi2p"),
             "f000000000000000000000000000000000000000000000000000000000000000000000000000000000000f");

    SUBCASE("empty hex") {
        CHECK_EQ(hashids.decode_hex(""), "");
    }

    SUBCASE("illegal") {
        CHECK_THROWS(
                hashids.decode_hex("WxMLpERDrmh25Lp4L3xEfM6WovWYO3IjkRMKR2ogCMVlqt1WK8jKq7OsEp1Vi2p"));
    }
}

// NOLINTEND(readability-magic-numbers)

TEST_SUITE_END();

} // namespace
