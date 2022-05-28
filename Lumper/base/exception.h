//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef BASE_EXCEPTION_H_
#define BASE_EXCEPTION_H_

#include <exception>
#include <type_traits>

#include "fmt/os.h"
#include "spdlog/spdlog.h"

namespace base {

template<typename E,
         typename... Args,
         std::enable_if_t<std::is_base_of_v<std::exception, E>, int> = 0>
void rethrow_as(const std::exception_ptr& eptr, Args&&... args) {
    try {
        std::rethrow_exception(eptr);
    } catch (const std::exception& ex) {
        throw E(ex.what(), std::forward<Args>(args)...);
    }
}

} // namespace base

#endif // BASE_EXCEPTION_H_
