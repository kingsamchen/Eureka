#pragma once

#ifndef RCP_UNIQUE_HANDLE_H_
#define RCP_UNIQUE_HANDLE_H_

#include <cstddef>
#include <memory>

#include "rcp/macros.h"

#if defined(OS_WIN)
#else
#include <unistd.h>
#endif

namespace rcp {

// unique_handle doesn't actually own the `handle_`, instead, its unique_ptr specialization
// does.
template<typename Traits>
class unique_handle {
public:
    using handle_type = typename Traits::handle_type;

    unique_handle() noexcept = default;

    // implicit
    unique_handle(std::nullptr_t) noexcept {}

    // implicit
    unique_handle(handle_type handle) noexcept
        : handle_(handle) {}

    explicit operator bool() const noexcept {
        return Traits::is_valid(handle_);
    }

    // implicit
    operator handle_type() const noexcept {
        return handle_;
    }

    static void close(handle_type handle) noexcept {
        Traits::close(handle);
    }

    friend bool operator==(unique_handle lhs, unique_handle rhs) noexcept {
        return lhs.handle_ == rhs.handle_;
    }

    friend bool operator!=(unique_handle lhs, unique_handle rhs) noexcept {
        return !(lhs == rhs);
    }

private:
    handle_type handle_{Traits::null_handle};
};

template<typename Traits>
struct unique_handle_deleter {
    using pointer = unique_handle<Traits>;

    void operator()(pointer ptr) const {
        pointer::close(ptr);
    }
};

#if defined(_WIN32) || defined(_WIN64)

struct win_handle_traits {
    using handle_type = HANDLE;

    static bool is_valid(handle_type handle) noexcept {
        return handle != nullptr && handle != INVALID_HANDLE_VALUE;
    }

    static void close(handle_type handle) noexcept {
        CloseHandle(handle);
    }

    static constexpr handle_type null_handle{nullptr};
};

using win_handle_deleter = unique_handle_deleter<win_handle_traits>;
using unique_win_handle = std::unique_ptr<win_handle_traits::handle_type, win_handle_deleter>;

#else

struct fd_traits {
    using handle_type = int;

    static bool is_valid(handle_type handle) noexcept {
        return handle != fd_traits::null_handle;
    }

    static void close(handle_type handle) noexcept {
        ::close(handle);
    }

    static constexpr handle_type null_handle{-1};
};

using fd_deleter = unique_handle_deleter<fd_traits>;
using unique_fd = std::unique_ptr<fd_traits::handle_type, fd_deleter>;

inline unique_fd wrap_unique_fd(fd_traits::handle_type raw_fd) {
    return unique_fd(raw_fd);
}

#endif

} // namespace rcp

#endif // RCP_UNIQUE_HANDLE_H_
