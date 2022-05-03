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

template<typename E, std::enable_if_t<std::is_base_of_v<std::exception, E>, int> = 0>
void rethrow_as(const std::exception_ptr& eptr, const std::string& failure) {
    try {
        std::rethrow_exception(eptr);
    } catch (const std::system_error& ex) {
        SPDLOG_ERROR("{}; cause={} code={}", failure, ex.what(), ex.code());
        throw E(ex.what());
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("{}; cause={}", failure, ex.what());
        throw E(ex.what());
    }
}

} // namespace base

#endif // BASE_EXCEPTION_H_
