//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "doctest/doctest.h"

#include <chrono>
#include <csignal>
#include <filesystem>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <vector>

#include <fcntl.h>
#include <sys/types.h>

#include "esl/strings.h"
#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "base/ignore.h"
#include "base/subprocess.h"

namespace {

namespace fs = std::filesystem;

std::string drain_fd(int fd) {
    std::string result;

    constexpr size_t buf_size = 64;
    char buf[buf_size] = {0};
    ssize_t bytes_read = 0;
    do {
        bytes_read = ::read(fd, buf, buf_size);
        if (bytes_read == -1) {
            throw std::system_error(errno, std::system_category(), "read fd");
        }
        result.append(buf, bytes_read);
    } while (bytes_read == buf_size);

    return result;
}

class touch_file_before_exec : public base::subprocess::evil_pre_exec_callback {
public:
    explicit touch_file_before_exec(std::string filename)
        : filename_(std::move(filename)) {}

    int run() noexcept override {
        int fd = ::open(filename_.c_str(), O_CREAT | O_WRONLY);
        if (fd == -1) {
            return errno;
        }

        ::close(fd);
        return 0;
    }

private:
    std::string filename_;
};

struct einval_fail_before_exec : public base::subprocess::evil_pre_exec_callback {
    int run() noexcept override {
        return EINVAL;
    }
};

TEST_SUITE_BEGIN("subprocess");

TEST_CASE("default constructed") {
    base::subprocess empty_proc;
    CHECK_FALSE(empty_proc.waitable());

    SUBCASE("stdio pipes are unavailable") {
        CHECK_EQ(-1, empty_proc.stdin_pipe());
        CHECK_EQ(-1, empty_proc.stdout_pipe());
        CHECK_EQ(-1, empty_proc.stderr_pipe());
    }
}

TEST_CASE("exits successfully") {
    base::subprocess proc({"/bin/true"});
    REQUIRE_GT(proc.pid(), 0);
    REQUIRE(proc.waitable());

    auto exit_code = proc.wait();
    CHECK_EQ(proc.pid(), -1);
    CHECK(exit_code.exited());

    auto cause = exit_code.cause();
    CHECK_EQ(cause.first, base::process_exit_code::reason::exited);
    CHECK_EQ(cause.second, 0);
}

TEST_CASE("exits with error") {
    base::subprocess proc({"/bin/false"});
    REQUIRE_GT(proc.pid(), 0);
    REQUIRE(proc.waitable());

    auto exit_code = proc.wait();
    CHECK_EQ(proc.pid(), -1);
    CHECK(exit_code.exited());

    auto cause = exit_code.cause();
    CHECK_EQ(cause.first, base::process_exit_code::reason::exited);
    CHECK_EQ(cause.second, 1);
}

TEST_CASE("child process is killed") {
    base::subprocess::options opts;
    opts.set_stdin(base::subprocess::use_pipe).set_stdout(base::subprocess::use_pipe);
    base::subprocess proc({"/bin/echo"}, opts);
    REQUIRE_GT(proc.pid(), 0);
    auto rv = ::kill(proc.pid(), SIGINT);
    CAPTURE(errno);
    REQUIRE_EQ(rv, 0);

    auto exit_code = proc.wait();
    CHECK_EQ(proc.pid(), -1);
    CHECK(exit_code.killed());

    auto cause = exit_code.cause();
    CHECK_EQ(cause.first, base::process_exit_code::reason::killed);
    CHECK_EQ(cause.second, SIGINT);
}

TEST_CASE("no stdio pipes by default") {
    base::subprocess proc({"/bin/true"});
    CHECK_EQ(-1, proc.stdin_pipe());
    CHECK_EQ(-1, proc.stdout_pipe());
    CHECK_EQ(-1, proc.stderr_pipe());
    base::ignore_unused(proc.wait());
}

TEST_CASE("use pipe for stdin/stdout") {
    base::subprocess::options opts;
    opts.set_stdin(base::subprocess::use_pipe).set_stdout(base::subprocess::use_pipe);
    base::subprocess proc({"/usr/bin/tr", "[:lower:]", "[:upper:]"}, opts);
    REQUIRE_NE(-1, proc.stdin_pipe());
    REQUIRE_NE(-1, proc.stdout_pipe());
    REQUIRE_EQ(-1, proc.stderr_pipe());

    INFO("errno=", errno);

    char sz[] = "hello\n";
    constexpr auto sz_len = std::size(sz) - 1;
    auto wr_n = ::write(proc.stdin_pipe(), sz, sz_len);
    REQUIRE_EQ(sz_len, wr_n);

    // Close stdin to indicate completion of input.
    proc.close_stdin_pipe();
    CHECK_EQ(-1, proc.stdin_pipe());

    auto rd_n = ::read(proc.stdout_pipe(), sz, sz_len);
    REQUIRE_EQ(sz_len, rd_n);

    CHECK_EQ(std::string_view(sz), "HELLO\n");

    auto cause = proc.wait().cause();
    CHECK_EQ(cause.first, base::process_exit_code::reason::exited);
    CHECK_EQ(cause.second, 0);
}

TEST_CASE("redirect stdio to /dev/null") {
    base::subprocess::options opts;
    opts.set_stdin(base::subprocess::use_null)
            .set_stderr(base::subprocess::use_null)
            .set_stdout(base::subprocess::use_pipe);
    base::subprocess proc({"/usr/bin/stat",
                           "-Lc",
                           "%t:%T",
                           "/dev/null",
                           "/dev/stdin",
                           "/dev/stderr"},
                          opts);
    REQUIRE_EQ(-1, proc.stdin_pipe());
    REQUIRE_EQ(-1, proc.stderr_pipe());
    REQUIRE_NE(-1, proc.stdout_pipe());

    auto output = drain_fd(proc.stdout_pipe());
    auto lines = esl::strings::split(output, '\n', esl::strings::skip_empty{})
                         .to<std::vector<std::string_view>>();
    CAPTURE(output);
    REQUIRE_EQ(3, lines.size());
    CHECK_EQ(lines[0], lines[1]);
    CHECK_EQ(lines[0], lines[2]);

    auto cause = proc.wait().cause();
    CHECK_EQ(cause.first, base::process_exit_code::reason::exited);
    CHECK_EQ(cause.second, 0);
}

TEST_CASE("failure during constructing") {
    SUBCASE("argv is empty") {
        // NOLINTNEXTLINE(readability-else-after-return)
        CHECK_THROWS_AS({ base::subprocess proc(std::vector<std::string>{}); },
                        std::invalid_argument);
    }

    SUBCASE("failed to exec child process") {
        // NOLINTNEXTLINE(readability-else-after-return)
        CHECK_THROWS_AS({ base::subprocess proc({"/no/such/file"}); },
                        base::spawn_subprocess_error);
    }
}

TEST_CASE("pre exec evil callback") {
    auto filename = fmt::format("/tmp/{}.test",
                                std::chrono::system_clock::now().time_since_epoch().count());
    REQUIRE_FALSE(fs::exists(filename));
    touch_file_before_exec tcb(filename);
    base::subprocess proc({"/bin/true"},
                          base::subprocess::options().set_evil_pre_exec_callback(&tcb));
    std::error_code ec;
    CHECK(fs::exists(filename, ec));
    CHECK_EQ(ec, std::error_code{});
    base::ignore_unused(proc.wait());
}

TEST_CASE("pre exec evil callback failed") {
    einval_fail_before_exec ecb;
    // NOLINTNEXTLINE(readability-else-after-return)
    CHECK_THROWS_AS({
        base::subprocess proc({"/bin/true"},
                              base::subprocess::options().set_evil_pre_exec_callback(&ecb));
    }, base::spawn_subprocess_error);
}

TEST_CASE("construct a subprocess by move") {
    base::subprocess proc({"/bin/echo", "hello"},
                          base::subprocess::options().set_stdout(base::subprocess::use_pipe));
    REQUIRE_GT(proc.pid(), 0);
    REQUIRE(proc.waitable());
    REQUIRE_NE(proc.stdout_pipe(), -1);

    auto cpid = proc.pid();
    auto pipe_fd = proc.stdout_pipe();

    base::subprocess new_proc(std::move(proc));
    CHECK_EQ(new_proc.pid(), cpid);
    CHECK(new_proc.waitable());
    CHECK_EQ(new_proc.stdout_pipe(), pipe_fd);
    CHECK_EQ(new_proc.stdin_pipe(), -1);
    CHECK_EQ(new_proc.stderr_pipe(), -1);

    // moved subprocess should be in default-inited state.
    CHECK_EQ(proc.pid(), -1);
    CHECK_FALSE(proc.waitable());
    CHECK_EQ(proc.stdin_pipe(), -1);
    CHECK_EQ(proc.stdout_pipe(), -1);
    CHECK_EQ(proc.stderr_pipe(), -1);

    base::ignore_unused(new_proc.wait());
}

TEST_CASE("move assign to a subprocess") {
    base::subprocess orig_proc({"/bin/true"});
    REQUIRE_GT(orig_proc.pid(), 0);
    REQUIRE(orig_proc.waitable());

    auto cpid = orig_proc.pid();

    SUBCASE("move-to is in default constructed") {
        base::subprocess new_proc;
        REQUIRE_EQ(new_proc.pid(), -1);
        REQUIRE_FALSE(new_proc.waitable());

        new_proc = std::move(orig_proc);
        CHECK_EQ(new_proc.pid(), cpid);
        CHECK(new_proc.waitable());

        CHECK_EQ(orig_proc.pid(), -1);
        CHECK_FALSE(orig_proc.waitable());

        base::ignore_unused(new_proc.wait());
    }

    SUBCASE("move-to is in exited state") {
        base::subprocess new_proc({"/bin/false"});
        REQUIRE_NE(new_proc.pid(), cpid);
        REQUIRE(new_proc.waitable());
        REQUIRE(new_proc.wait().exited());

        new_proc = std::move(orig_proc);
        CHECK_EQ(new_proc.pid(), cpid);
        CHECK(new_proc.waitable());

        CHECK_EQ(orig_proc.pid(), -1);
        CHECK_FALSE(orig_proc.waitable());

        base::ignore_unused(new_proc.wait());
    }
}

TEST_SUITE_END;

} // namespace
