
#ifndef SCOPED_HANDLE_H_
#define SCOPED_HANDLE_H_

#include <cstdio>
#include <memory>
#include <type_traits>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace scoped {
namespace details {

template<typename T>
void ignore_result(T&&) {}

} // namespace details

struct c_file_deleter {
    void operator()(FILE* ptr) const {
        fclose(ptr);
    }
};

static_assert(std::is_empty_v<c_file_deleter>);

using scoped_file = std::unique_ptr<FILE, c_file_deleter>;

// -*- For Win32 HANDLE -*-
#if defined(_WIN32) || defined(_WIN64)

class win_handle {
public:
    win_handle() noexcept = default;

    // implicit
    win_handle(std::nullptr_t) noexcept {}

    // implicit
    win_handle(HANDLE h)
        : handle_(h) {}

    explicit operator bool() const noexcept {
        return handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE;
    }

    // implicit
    operator HANDLE() const noexcept {
        return handle_;
    }

    friend bool operator==(win_handle lhs, win_handle rhs) noexcept {
        return lhs.handle_ == rhs.handle_;
    }

    friend bool operator!=(win_handle lhs, win_handle rhs) noexcept {
        return !(lhs == rhs);
    }

private:
    HANDLE handle_ = nullptr;
};

struct win_handle_deleter {
    using pointer = win_handle;

    void operator()(pointer ptr) const {
        details::ignore_result(CloseHandle(ptr));
    }
};

static_assert(std::is_empty_v<win_handle_deleter>);

using scoped_win_handle = std::unique_ptr<win_handle, win_handle_deleter>;

#else

class fd_handle {
public:
    using handle_type = int;

    fd_handle() noexcept = default;

    // implicit
    fd_handle(std::nullptr_t) noexcept {}

    // implicit
    fd_handle(handle_type fd) noexcept
        : fd_(fd) {}

    explicit operator bool() const noexcept {
        return fd_ != 0;
    }

    // implicit
    operator handle_type() const noexcept {
        return fd_;
    }

    friend bool operator==(fd_handle lhs, fd_handle rhs) noexcept {
        return lhs.fd_ == rhs.fd_;
    }

    friend bool operator!=(fd_handle lhs, fd_handle rhs) noexcept {
        return !(lhs == rhs);
    }

private:
    handle_type fd_{};
};

struct fd_handle_deleter {
    using pointer = fd_handle;

    void operator()(pointer fd) const {
        close(fd);
    }
};

static_assert(std::is_empty_v<fd_handle_deleter>);

using scoped_fd = std::unique_ptr<fd_handle, fd_handle_deleter>;

#endif

} // namespace scoped

#endif // SCOPED_HANDLE_H_
