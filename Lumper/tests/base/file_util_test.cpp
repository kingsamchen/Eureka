//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "doctest/doctest.h"

#include <chrono>
#include <fstream>

#include "fmt/format.h"

#include "base/file_util.h"

namespace {

TEST_SUITE_BEGIN("file_util");

TEST_CASE("write data to file") {
    auto ts = std::chrono::system_clock::now().time_since_epoch().count();
    auto filename = fmt::format("/tmp/test_write_file_{}.txt", ts);
    constexpr char data[] = "This is a test text";
    base::write_to_file(filename, data);
    // Read from file
    std::ifstream in(filename);
    REQUIRE(in.is_open());
    std::stringstream ss;
    ss << in.rdbuf();
    auto read_data = ss.str();
    CHECK_EQ(read_data, data);
}

TEST_CASE("throws when writing file failed") {
    constexpr char data[] = "This is a test text";

    SUBCASE("failure when (parent) path doesn't exist") {
        CHECK_THROWS_AS( // NOLINT(readability-else-after-return)
                base::write_to_file("/tmp/no/such/dir/exist", data),
                std::filesystem::filesystem_error);
    }

    SUBCASE("faialure when no permission to write") {
        CHECK_THROWS_AS( // NOLINT(readability-else-after-return)
                base::write_to_file("/var/log/test_write_file.txt", data),
                std::filesystem::filesystem_error);
    }
}

TEST_SUITE_END();

} // namespace
