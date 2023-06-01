
#include <filesystem>
#include <random>
#include <string_view>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "esl/strings.h"

#include "file_io/file_io.h"

namespace fs = std::filesystem;

using namespace std::string_view_literals;

inline std::string new_test_filepath() {
    auto tmp_dir = fs::temp_directory_path();
    auto ts = std::chrono::system_clock::now().time_since_epoch().count();
    auto rnd = std::random_device{}();
    std::string filename{"test_"};
    filename.append(std::to_string(ts)).append("_").append(std::to_string(rnd)).append(".txt");
    return (tmp_dir / filename).string();
}

int main(int argc, const char* argv[]) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

TEST_CASE("write file then read") {
    auto file = new_test_filepath();
    CAPTURE(file);
    const std::string str{"this is a test text\n"};
    llio::write_to_file(file, str);
    std::string read_content;
    llio::read_from_file(file, read_content);
    CHECK_EQ(read_content, str);
}

TEST_CASE("overwrite if file already exists") {
    auto file = new_test_filepath();
    CAPTURE(file);

    std::string read_content;
    constexpr auto s1 = "this is a test text"sv;
    llio::write_to_file(file, s1);
    llio::read_from_file(file, read_content);
    CHECK_EQ(read_content, s1);

    // Overwrite the file content.
    constexpr auto s2 = "a quick fox jumps over a lazy dog"sv;
    REQUIRE_NE(s2, s1);
    llio::write_to_file(file, s2);
    llio::read_from_file(file, read_content);
    CHECK_EQ(read_content, s2);
}

TEST_CASE("write to the file don't have write permission") {
    auto file = new_test_filepath();
    CAPTURE(file);

    // Create the file first.
    std::ofstream out(file);
    out.close();

    // Remove owner write permission.
    // NOTE: must remove all three write permissions on Windows.
    REQUIRE((fs::status(file).permissions() & fs::perms::owner_write) != fs::perms::none);
    auto write_perms = fs::perms::owner_write | fs::perms::others_write | fs::perms::group_write;
    fs::permissions(file, write_perms, fs::perm_options::remove);
    REQUIRE((fs::status(file).permissions() & fs::perms::owner_write) == fs::perms::none);

    // Write to the file should fail.
    std::error_code ec;
    llio::write_to_file(file, "abcdefg", ec);
    INFO("ec.value=", ec.value(), " ec.message=", ec.message());
    CHECK(static_cast<bool>(ec));
    CHECK_EQ(ec, std::errc::permission_denied);
}

TEST_CASE("read file that does not exist") {
    // Generate the path but not to create the file.
    auto file = new_test_filepath();
    std::error_code ec;
    std::string content;
    llio::read_from_file(file, content, ec);
    CHECK(static_cast<bool>(ec));
    CHECK_EQ(ec, std::errc::no_such_file_or_directory);
    CHECK(content.empty());
}

TEST_CASE("read file that don't have read permission") {
    auto file = new_test_filepath();
    CAPTURE(file);

    // Create the file first.
    std::ofstream out(file);
    out.close();

    // Remove read permission.
    REQUIRE((fs::status(file).permissions() & fs::perms::owner_read) != fs::perms::none);
    fs::permissions(file, fs::perms::owner_read, fs::perm_options::remove);
    REQUIRE((fs::status(file).permissions() & fs::perms::owner_read) == fs::perms::none);

    // Read file should fail.
    std::error_code ec;
    std::string content;
    llio::read_from_file(file, content, ec);
    INFO("ec.value=", ec.value(), " ec.message=", ec.message());
    CHECK(static_cast<bool>(ec));
    CHECK_EQ(ec, std::errc::permission_denied);
}

TEST_CASE("read a special file") {
    const std::filesystem::path file{"/proc/cgroups"};
    REQUIRE(std::filesystem::exists(file));
    REQUIRE_EQ(std::filesystem::file_size(file), 0U);
    std::string content;
    llio::read_from_file(file.native(), content);
    CHECK_FALSE(content.empty());
    CAPTURE(content);
    CHECK(esl::strings::starts_with(content, "#subsys_name"));
}
