
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
    std::transform(s.begin(), s.end(), std::back_inserter(out), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });
    return out;
}

void raw_write_line(rcp::pipe::native_handle_t fd, std::string_view data) {
    std::string line(data);
    line += '\n';
#if defined(OS_WIN)
    [[maybe_unused]] DWORD bytes_written = 0;
    if (!::WriteFile(fd, line.data(), static_cast<DWORD>(line.size()), &bytes_written, nullptr)) {
        throw std::system_error(std::error_code(::GetLastError(), std::system_category()),
                                "WriteFile failure");
    }
#else
    if (::write(fd, line.data(), line.size()) != static_cast<ssize_t>(line.size())) {
        throw std::system_error(std::error_code(errno, std::system_category()),
                                "write failure");
    }
#endif
}

std::string raw_read_line(rcp::pipe::native_handle_t fd) {
    constexpr size_t buf_size = 256;
    char buf[buf_size]{};
    std::string line;
    for (long bytes_read;;) {
#if defined(OS_WIN)
        if (!::ReadFile(fd, buf, buf_size, reinterpret_cast<LPDWORD>(&bytes_read), nullptr)) {
            throw std::system_error(std::error_code(::GetLastError(), std::system_category()),
                                    "ReadFile failure");
        }
#else
        bytes_read = ::read(fd, buf, buf_size);
        if (bytes_read <= 0) {
            throw std::system_error(std::error_code(errno, std::system_category()),
                                    "read failure");
        }
#endif
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
#if defined(OS_WIN)
    constexpr char app_name[] = "rcp_app.exe";
#else
    constexpr char app_name[] = "rcp_app";
#endif

    REQUIRE(g_argc > 0);
    std::filesystem::path cur(g_argv[0]);
    auto app_path = cur.parent_path().append(app_name);
    CAPTURE(app_path);
    REQUIRE(std::filesystem::exists(app_path));
    auto rcp_app = rcp::run_child_process(app_path.string());

    static constexpr std::string_view stmts[]{"hello, world",
                                              "HELLO, WORLD",
                                              "hElLo, WOrlD"};
    for (auto stmt : stmts) {
        raw_write_line(rcp_app.std_in(), stmt);
        auto data = raw_read_line(rcp_app.std_out());
        INFO("stmt=", stmt, "\ndata=", data);
        CHECK_EQ(data, strupper(stmt));
    }

    rcp_app.shutdown();
    rcp_app.wait();
}

} // namespace

int main(int argc, const char* argv[]) {
    g_argc = argc;
    g_argv = argv;
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
