#pragma once

#ifndef RCP_PIPE_H_
#define RCP_PIPE_H_

#include <system_error>

#include "rcp/macros.h"

#if defined(OS_WIN)
#else
#include <unistd.h>
#endif

#include "rcp/unique_handle.h"

namespace rcp {

class pipe {
public:
#if defined(OS_WIN)
    using unique_handle_t = unique_win_handle;
#else
    using unique_handle_t = unique_fd;
#endif

    using native_handle_t = unique_handle_t::pointer::handle_type;

    pipe() = default;

    pipe(pipe&&) = default;

    pipe(unique_handle_t read_end, unique_handle_t write_end)
        : read_end_(std::move(read_end)),
          write_end_(std::move(write_end)) {}

    pipe& operator=(pipe&&) = default;

    ~pipe() = default;

    pipe(const pipe&) = delete;

    pipe& operator=(const pipe&) = delete;

    const unique_handle_t& read_end() const noexcept {
        return read_end_;
    }

    unique_handle_t& read_end() noexcept {
        return read_end_;
    }

    const unique_handle_t& write_end() const noexcept {
        return write_end_;
    }

    unique_handle_t& write_end() noexcept {
        return write_end_;
    }

    void reset(unique_handle_t read_end = nullptr, unique_handle_t write_end = nullptr) noexcept {
        read_end_ = std::move(read_end);
        write_end_ = std::move(write_end);
    }

private:
    unique_handle_t read_end_;
    unique_handle_t write_end_;
};

inline pipe make_pipe() {
#if defined(OS_WIN)
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    HANDLE fds[2]{};
    // use default buffer size.
    if (::CreatePipe(&fds[0], &fds[1], &sa, 0) == 0) {
        throw std::system_error(std::error_code(::GetLastError(), std::system_category()),
                                "CreatePipe() failure");
    }
    return pipe(wrap_unique_win_handle(fds[0]), wrap_unique_win_handle(fds[1]));
#else
    int fds[2]{};
    if (::pipe(fds) != 0) {
        throw std::system_error(std::error_code(errno, std::system_category()),
                                "pipe() failure");
    }
    return pipe(wrap_unique_fd(fds[0]), wrap_unique_fd(fds[1]));
#endif
}

} // namespace rcp

#endif // RCP_PIPE_H_
