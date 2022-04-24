//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "base/subprocess.h"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <system_error>
#include <utility>

#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#include "fmt/core.h"
#include "fmt/os.h"
#include "spdlog/spdlog.h"

#define FORCE_AS_MEMBER_FUNCTION() \
    ::base::ignore_unused(this)

namespace base {
namespace {

template<typename E>
auto enum_cast(E e) noexcept -> std::underlying_type_t<E> {
    return static_cast<std::underlying_type_t<E>>(e);
}

template<typename T>
void ignore_unused(T&&) {}

enum class child_errc : std::int32_t {
    success = 0,
    prepare_stdio,
    exec_call_failure
};

struct child_error_info {
    std::underlying_type_t<child_errc> err_code;
    int32_t errno_value;
};

std::string stringify_child_error_info(const char* exe, child_error_info info) {
    constexpr const char* errc_msgs[] = {"success",
                                         "failed to prepare stdio fd",
                                         "failed to call exec"};
    assert(0 <= info.err_code && info.err_code < std::size(errc_msgs));
    return fmt::format("cannot spawn {}: {}; errno={}",
                       exe, errc_msgs[info.err_code], info.errno_value);
}

static_assert(sizeof(child_error_info) == 8);

void check_system_error(int ret, const char* what) {
    if (ret == -1) {
        throw std::system_error(errno, std::system_category(), what);
    }
}

std::pair<esl::unique_fd, esl::unique_fd> make_pipe() {
    int fds[2]{};
    auto rv = ::pipe2(fds, O_CLOEXEC);
    check_system_error(rv, "failed to pipe2()");
    return {esl::wrap_unique_fd(fds[0]), esl::wrap_unique_fd(fds[1])};
}

// The function returns only if an error has occurred.
int run_child_executable(const char* file, const char* argv[]) {
    ::execvp(file, const_cast<char**>(argv)); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    return errno;
}

[[noreturn]] void notify_child_error(int err_fd, child_errc err_code, int errno_value) {
    child_error_info err{enum_cast(err_code), errno_value};

    // Since we are writing 8-byte into a blocking pipe, the write may block,
    // but once it completes successfully, no short write will ever happen.
    ssize_t wc = 0;
    do {
        wc = ::write(err_fd, &err, sizeof(err));
    } while (wc == -1 && errno == EINTR);

    std::_Exit(static_cast<int>(err_code));
}

} // namespace

spawn_subprocess_error::spawn_subprocess_error(const char* exe, std::int32_t error_code,
                                               int errno_value)
    : std::runtime_error(stringify_child_error_info(exe, {error_code, errno_value})),
      errno_value_(errno_value) {}

//
// process_exit_code
//

// static
process_exit_code process_exit_code::make(int wait_status) {
    if (!WIFEXITED(wait_status) && !WIFSIGNALED(wait_status)) {
        throw std::runtime_error(fmt::format("invalid wait status: {}", wait_status));
    }

    return process_exit_code(wait_status);
}

auto process_exit_code::cause() const -> std::pair<reason, int> {
    if (WIFEXITED(wait_status_)) {
        return {reason::exited, WEXITSTATUS(wait_status_)};
    }

    if (WIFSIGNALED(wait_status_)) {
        return {reason::killed, WTERMSIG(wait_status_)};
    }

    SPDLOG_CRITICAL("Invalid process exit code; status={}", wait_status_);
    std::terminate();
}

//
// subprocess
//

subprocess::subprocess(const std::vector<std::string>& argv, const options& opts) {
    if (argv.empty()) {
        throw std::invalid_argument("args cannot be empty");
    }

    std::unique_ptr<const char*[]> argvp(new const char*[argv.size() + 1]);
    for (size_t i = 0; i < argv.size(); ++i) {
        argvp[i] = argv[i].c_str();
    }
    argvp[argv.size()] = nullptr;

    options clone_opts(opts);

    spawn(std::move(argvp), clone_opts);
}

subprocess& subprocess::operator=(subprocess&& rhs) noexcept {
    if (waitable()) {
        SPDLOG_CRITICAL("Current instance still has a running child process");
        std::terminate();
    }

    child_state_ = std::exchange(rhs.child_state_, state::not_started);
    pid_ = std::exchange(rhs.pid_, -1);
    for (auto i = 0; i < std::size(stdio_pipes_); ++i) {
        stdio_pipes_[i] = std::move(rhs.stdio_pipes_[i]);
        rhs.stdio_pipes_[i].reset();
    }

    return *this;
}

subprocess::~subprocess() {
    if (waitable()) {
        SPDLOG_CRITICAL("Current instance still has a running child process");
        std::terminate();
    }
}

void subprocess::spawn(std::unique_ptr<const char*[]> argvp, options& opts) {
    // Retain lifetime of pipe fd for the child process.
    std::vector<esl::unique_fd> child_pipe_fds;
    child_pipe_fds.reserve(3);

    for (auto& [sfd, action] : opts.action_table_) {
        auto ptr = std::get_if<use_pipe_t>(&action);
        if (ptr) {
            auto pipe_fds = make_pipe();
            if (ptr->mode == use_pipe_t::mode::in) {
                stdio_pipes_[sfd] = std::move(pipe_fds.second);
                child_pipe_fds.emplace_back(std::move(pipe_fds.first));
            } else {
                stdio_pipes_[sfd] = std::move(pipe_fds.first);
                child_pipe_fds.emplace_back(std::move(pipe_fds.second));
            }
            ptr->pfd = child_pipe_fds.back().get();
        }
    }

    auto [err_pipe_rd, err_pipe_wr] = make_pipe();

    spawn_impl(argvp.get(), opts, err_pipe_wr.get());

    // Child's error pipe write end will be closed on exec(), and we must close parent's
    // write end as well before read. Because if child process executed successfully, no
    // data will be sent, and read in parent will block.
    err_pipe_wr.reset();
    read_child_error_pipe(err_pipe_rd.get(), argvp[0]);
}

void subprocess::spawn_impl(const char* argvp[], const options& opts, int err_fd) {
    auto pid = static_cast<pid_t>(::syscall(SYS_clone, opts.clone_flags_, 0, nullptr, nullptr));
    check_system_error(pid, "failed to clone");

    // Within child process.
    if (pid == 0) {
        try {
            for (const auto& action : opts.action_table_) {
                std::visit(
                        [this, sfd = action.first](const auto& act) {
                            handle_stdio_action(sfd, act);
                        },
                        action.second);
            }
        } catch (const std::system_error& ex) {
            SPDLOG_ERROR("Failed to prepare stdio fd for child process; ec={} what={}",
                         ex.code(), ex.what());
            notify_child_error(err_fd, child_errc::prepare_stdio, ex.code().value());
        }

        auto errno_value = run_child_executable(*argvp, argvp);
        notify_child_error(err_fd, child_errc::exec_call_failure, errno_value);
    }

    // Now we are done.
    child_state_ = state::running;
    pid_ = pid;
}

void subprocess::read_child_error_pipe(int err_fd, const char* executable) {
    child_error_info err_info{};

    ssize_t rc = 0;
    do {
        rc = ::read(err_fd, &err_info, sizeof(err_info));
    } while (rc == -1 && errno == EINTR);

    // Child executed successfully.
    if (rc == 0) {
        return;
    }

    // Read pipe failure or partial read.
    // We can do nothing in this case and we don't know what exactly happened, just
    // pretend the child has succeeded and return normally.
    if (rc != sizeof(err_info)) {
        SPDLOG_ERROR("Failed to read from child error pipe; rc={} errno={}", rc, errno);
        return;
    }

    // We now are sure that child has failed.
    // Wait it to exit.
    wait();

    // Signal the spawn failure.
    throw spawn_subprocess_error(executable, err_info.err_code, err_info.errno_value);
}

void subprocess::handle_stdio_action(int stdio_fd, const use_pipe_t& action) {
    FORCE_AS_MEMBER_FUNCTION();

    int rv = ::dup2(action.pfd, stdio_fd);
    check_system_error(rv, "failed to dup pipe fd");
}

void subprocess::handle_stdio_action(int stdio_fd, const use_null_t& action) {
    FORCE_AS_MEMBER_FUNCTION();

    constexpr char dev_null_path[] = "/dev/null";

    int flags = O_CLOEXEC;
    flags |= action.mode == use_null_t::mode::in ? O_RDONLY : O_WRONLY;
    auto fd = esl::wrap_unique_fd(::open(dev_null_path, flags));
    check_system_error(fd.get(), "failed to open /dev/null");

    int rv = ::dup2(fd.get(), stdio_fd);
    check_system_error(rv, "failed to dup null dev fd");
}

process_exit_code subprocess::wait() {
    if (!waitable()) {
        throw std::invalid_argument("subprocess is not waitable");
    }

    int status = 0;
    pid_t waited_pid = -1;
    do {
        waited_pid = ::waitpid(pid_, &status, 0);
    } while (waited_pid == -1 && errno == EINTR);

    // Cannot throw here, because we know nothing about the child process, and there is
    // nothing we can do to maintain the class invariance.
    // This failure should rarely happen in practice, just abort.
    if (waited_pid == -1) {
        SPDLOG_CRITICAL("Failed to wait child process; errno={}", errno);
        std::terminate();
    }

    if (waited_pid != pid_) {
        SPDLOG_ERROR("Failed to verify waited child pid; pid_={} waited={}", pid_, waited_pid);
    }

    // The child process has exited anyway.
    child_state_ = state::exited;
    pid_ = -1;
    assert(waitable() == false);

    return process_exit_code::make(status);
}

} // namespace base
