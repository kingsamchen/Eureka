// Kingsley Chen

#pragma once

#include <cstddef>
#include <string_view>

namespace http {

struct find_result {
    std::string_view wildcard_name;
    std::size_t pos{std::string_view::npos};

    [[nodiscard]] constexpr bool found() const noexcept {
        return pos != std::string_view::npos;
    }

    [[nodiscard]] constexpr bool valid_name() const noexcept {
        return !wildcard_name.empty();
    }
};

// Find a wildcard segment if existed and retrieve the wildcard name if valid.
// A wildcard segment starts with `:` or `*` and the name cannot contains `:` and `*`.
constexpr find_result find_wildcard(std::string_view path) {
    const auto start = path.find_first_of(":*");
    if (start == std::string_view::npos) {
        return find_result{};
    }

    const auto stop = path.find_first_of(":*/", start + 1);
    if (stop == std::string_view::npos) {
        return find_result{.wildcard_name = path.substr(start), .pos = start};
    }

    if (path[stop] == '/') {
        return find_result{.wildcard_name = path.substr(start, stop - start), .pos = start};
    }

    return find_result{.wildcard_name = std::string_view{}, .pos = start};
}

} // namespace http
