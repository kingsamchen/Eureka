
#include "rcp/rcp.h"

#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

namespace {

int g_argc = 0;
const char** g_argv = nullptr;

std::string strupper(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    std::transform(s.begin(), s.end(), std::back_inserter(out), toupper);
    return out;
}

void raw_write_line(rcp::pipe::native_handle_t fd, std::string_view data) {
    std::string line(data);
    line += '\n';
    if (::write(fd, line.data(), line.size()) != static_cast<ssize_t>(line.size())) {
        throw std::system_error(std::error_code(errno, std::system_category()),
                                "write failure");
    }
}

std::string raw_read_line(rcp::pipe::native_handle_t fd) {
    constexpr size_t buf_size = 256;
    char buf[buf_size]{};
    std::string line;
    for (long bytes_read = 0;;) {
        bytes_read = ::read(fd, buf, buf_size);
        if (bytes_read <= 0) {
            throw std::system_error(std::error_code(errno, std::system_category()),
                                    "read failure");
        }
        line.append(buf, bytes_read);
        if (static_cast<size_t>(bytes_read) < buf_size) {
            if (line.back() == '\n') {
                line.pop_back();
            }
            return line;
        }
    }
}

TEST_CASE("run rcp_app as child process and communicate via stdin/stdout") {
    REQUIRE(g_argc > 0);
    std::filesystem::path cur(g_argv[0]);
    auto app_path = cur.parent_path().append("rcp_app");
    REQUIRE(std::filesystem::exists(app_path));
    auto rcp_app = rcp::run_child_process(app_path.string());

    static constexpr std::string_view stmts[]{"hello, world",
                                              "HELLO, WORLD",
                                              "hElLo, WOrlD"};
    for (auto i = 0u; i < std::size(stmts); ++i) {
        raw_write_line(rcp_app.stdin(), stmts[i]);
        auto data = raw_read_line(rcp_app.stdout());
        CHECK_EQ(data, strupper(stmts[i]));
    }

    rcp_app.shutdown();
    rcp_app.join();
}

} // namespace

int main(int argc, const char* argv[]) {
    g_argc = argc;
    g_argv = argv;
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
