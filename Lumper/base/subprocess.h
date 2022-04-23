//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef BASE_SUBPROCESS_H_
#define BASE_SUBPROCESS_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <unistd.h>

#include "esl/unique_handle.h"

namespace base {

class spawn_subprocess_error : public std::runtime_error {
public:
    spawn_subprocess_error(const char* exe, std::int32_t error_code, int errno_value);

    int errno_value() const noexcept {
        return errno_value_;
    }

private:
    int errno_value_;
};

class process_exit_code {
public:
    enum class reason {
        exited = 1,
        killed
    };

    // Throws `runtime_error` if unable make from wait_status.
    static process_exit_code make(int wait_status);

    std::pair<reason, int> cause() const;

    bool exited() const noexcept {
        return WIFEXITED(wait_status_);
    }

    bool killed() const noexcept {
        return WIFSIGNALED(wait_status_);
    }

private:
    explicit process_exit_code(int wait_status)
        : wait_status_(wait_status) {}

private:
    int wait_status_;
};

class subprocess {
private:
    enum class state {
        not_started,
        running,
        exited
    };

    struct use_null_t {
        struct tag {};

        enum class mode {
            in,
            out
        };

        mode mode;
    };

    struct use_pipe_t {
        struct tag {};

        enum class mode {
            in,
            out
        };

        mode mode;
        int pfd{};
    };

public:
    static constexpr use_null_t::tag use_null;
    static constexpr use_pipe_t::tag use_pipe;

    class options {
        friend class subprocess;

    public:
        // See https://bit.ly/3rxamLo.
        options() {} // NOLINT(modernize-use-equals-default)

        options& clone_with_flags(std::uint64_t flags) noexcept {
            clone_flags_ = flags;
            return *this;
        }

        options& set_stdin(use_null_t::tag) {
            action_table_[STDIN_FILENO] = use_null_t{use_null_t::mode::in};
            return *this;
        }

        options& set_stdin(use_pipe_t::tag) {
            action_table_[STDIN_FILENO] = use_pipe_t{use_pipe_t::mode::in};
            return *this;
        }

        options& set_stdout(use_null_t::tag) {
            action_table_[STDOUT_FILENO] = use_null_t{use_null_t::mode::out};
            return *this;
        }

        options& set_stdout(use_pipe_t::tag) {
            action_table_[STDOUT_FILENO] = use_pipe_t{use_pipe_t::mode::out};
            return *this;
        }

        options& set_stderr(use_null_t::tag) {
            action_table_[STDERR_FILENO] = use_null_t{use_null_t::mode::out};
            return *this;
        }

        options& set_stderr(use_pipe_t::tag) {
            action_table_[STDERR_FILENO] = use_pipe_t{use_pipe_t::mode::out};
            return *this;
        }

    private:
        using stdio_action = std::variant<use_null_t, use_pipe_t>;
        std::uint64_t clone_flags_{};
        // TODO(KC): can replace with flatmap or ordered vector.
        std::map<int, stdio_action> action_table_;
    };

    // Spawn a process to run given commandline args.
    // `args[0]` must be fullpath to the executable.
    // Throws:
    //  - `std::invalid_argument` if `argv` is empty.
    //  - `std::system_error` for system related failures.
    //  - `spawn_subprocess_error` for spawning child process failure
    explicit subprocess(const std::vector<std::string>& argv, const options& opts = options());

    ~subprocess();

    // Not copyable but movable, like std::thread.
    // TODO(KC): explicit control move semantics over state.

    subprocess(const subprocess&) = delete;

    subprocess& operator=(const subprocess&) = delete;

    subprocess(subprocess&&) noexcept = default;

    subprocess& operator=(subprocess&&) noexcept = default;

    // Throws:
    //  - `invalid_argument` if `waitable()` is false.
    //  - `runtime_error` if unable to construct process exit code.
    process_exit_code wait();

    bool waitable() const noexcept {
        return child_state_ == state::running;
    }

    // Returns -1 i.e. invalid fd if no corresponding pipe was set.

    int stdin_pipe() const {
        return stdio_pipes_[STDIN_FILENO].get();
    }

    int stdout_pipe() const {
        return stdio_pipes_[STDOUT_FILENO].get();
    }

    int stderr_pipe() const {
        return stdio_pipes_[STDERR_FILENO].get();
    }

private:
    void spawn(std::unique_ptr<const char*[]> argvp, options& opts);

    void spawn_impl(const char* argvp[], const options& opts, int err_fd);

    void read_child_error_pipe(int err_fd, const char* executable);

    void handle_stdio_action(int stdio_fd, const use_null_t& action);

    void handle_stdio_action(int stdio_fd, const use_pipe_t& action);

private:
    state child_state_{state::not_started};
    pid_t pid_{-1};
    esl::unique_fd stdio_pipes_[3]{};
};

} // namespace base

#endif // BASE_SUBPROCESS_H_
