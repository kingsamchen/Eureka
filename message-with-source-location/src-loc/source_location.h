//
// Kingsley Chen
//

#pragma once

#ifndef SRC_LOC_SOURCE_LOCATION_H_
#define SRC_LOC_SOURCE_LOCATION_H_

#include <cstdint>
#include <string>
#include <string_view>

#include "fmt/format.h"

namespace base {

class source_location {
public:
    constexpr source_location() noexcept = default;

    // No consteval in C++ 17.
    [[nodiscard]] static constexpr source_location current(
            std::uint_least32_t line = __builtin_LINE(),
            const char* file = __builtin_FILE(),
            const char* func = __builtin_FUNCTION()) noexcept {
        source_location loc;
        loc.line_ = line;
        loc.file_name_ = file;
        loc.fn_name_ = func;
        return loc;
    }

    [[nodiscard]] constexpr std::uint_least32_t line() const noexcept {
        return line_;
    }

    [[nodiscard]] constexpr const char* file_name() const noexcept {
        return file_name_;
    }

    [[nodiscard]] constexpr const char* function_name() const noexcept {
        return fn_name_;
    }

private:
    std::uint_least32_t line_{0};
    const char* file_name_{nullptr};
    const char* fn_name_{nullptr};
};

[[nodiscard]] inline std::string msg_with_src_loc(
        std::string_view msg,
        source_location loc = source_location::current()) {
    return fmt::format("{}:{} {}(): {}",
                       loc.file_name(), loc.line(), loc.function_name(), msg);
}

} // namespace base

#endif // SRC_LOC_SOURCE_LOCATION_H_
