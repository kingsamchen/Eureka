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
#include <variant>
#include <vector>

#include <unistd.h>

namespace base {

class subprocess {
private:
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
    };

public:
    static constexpr use_null_t::tag use_null;
    static constexpr use_pipe_t::tag use_pipe;

    class options {
        friend class subprocess;

    public:
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

    private:
        using stdio_action = std::variant<use_null_t, use_pipe_t>;
        std::uint64_t clone_flags_{};
        // TODO(KC): can replace with flatmap or ordered vector.
        std::map<int, stdio_action> action_table_;
    };

    // Spawn a process to run given commandline args.
    // `args[0]` must be fullpath to the executable.
    // Throws `std::invalid_argument` if args is empty.
    explicit subprocess(const std::vector<std::string>& argv, const options& opts = options());

    ~subprocess() = default;

    // Not copyable but movable, like std::thread.

    subprocess(const subprocess&) = delete;

    subprocess& operator=(const subprocess&) = delete;

    subprocess(subprocess&&) noexcept = default;

    subprocess& operator=(subprocess&&) noexcept = default;

    void wait();

private:
    void spawn(std::unique_ptr<const char*[]> argvp, const options& opts);

private:
    pid_t pid_{};
};

} // namespace base

#endif // BASE_SUBPROCESS_H_
