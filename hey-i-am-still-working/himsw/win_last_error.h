// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#pragma once

#ifndef HIMSW_WIN_LAST_ERROR_H_
#define HIMSW_WIN_LAST_ERROR_H_

#include <stdexcept>

#include <errhandlingapi.h>

namespace himsw {

class win_last_error : public std::runtime_error {
public:
    using error_code_type = unsigned long;

    explicit win_last_error(const char* msg)
        : runtime_error(msg),
          error_code_(::GetLastError()) {}

    [[nodiscard]] error_code_type error_code() const noexcept {
        return error_code_;
    }

private:
    error_code_type error_code_;
};

} // namespace himsw

#endif // HIMSW_WIN_LAST_ERROR_H_
