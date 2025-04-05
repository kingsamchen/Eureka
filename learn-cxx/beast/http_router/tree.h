// Kingsley Chen

#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

#include <esl/ignore_unused.h>
#include <fmt/format.h>

namespace http {

struct wildcard_result {
    std::string_view name;
    std::size_t pos{std::string_view::npos};

    [[nodiscard]] constexpr bool found() const noexcept {
        return pos != std::string_view::npos;
    }

    [[nodiscard]] constexpr bool valid_name() const noexcept {
        return name.size() > 1;
    }
};

inline constexpr wildcard_result unknown_wildcard;

// Find a wildcard segment if existed and then retrieve the wildcard name if valid.
// A wildcard segment starts with `:` or `*` and the name cannot contain `:` and `*`.
// The name is always invalid if no wildcard found.
[[nodiscard]] constexpr wildcard_result find_wildcard(std::string_view path) {
    const auto start = path.find_first_of(":*");
    if (start == std::string_view::npos) {
        return wildcard_result{};
    }

    const auto stop = path.find_first_of(":*/", start + 1);
    if (stop == std::string_view::npos) {
        return wildcard_result{.name = path.substr(start), .pos = start};
    }

    if (path[stop] == '/') {
        return wildcard_result{.name = path.substr(start, stop - start), .pos = start};
    }

    return wildcard_result{.name = std::string_view{}, .pos = start};
}

// Returns length of common prefix.
[[nodiscard]] constexpr std::size_t longest_common_prefix(std::string_view s1,
                                                          std::string_view s2) {
    auto iters = std::ranges::mismatch(s1, s2);
    return static_cast<std::size_t>(std::distance(s1.begin(), iters.in1));
}

class node {
    enum class type : std::uint8_t {
        plain = 0,
        root,
        param,
        catch_all,
    };

public:
    void add_route(std::string_view path) {
        // The sub-tree rooted by the node has one more route.
        ++priority_;

        if (path_.empty() && indices_.empty()) {
            insert_path(path, path, wildcard_result{});
            type_ = type::root;
            return;
        }

        throw std::runtime_error("not implemented yet");
    }

private:
    void insert_path(std::string_view path, std::string_view full_path, wildcard_result wildcard) {
        // The invocation with no wildcard means we haven't scanned the `path` yet.
        // Let's scan the path in flight.
        if (!wildcard.found()) {
            wildcard = find_wildcard(path);
            if (!wildcard.found()) {
                path_ = path;
                return;
            }

            if (!wildcard.valid_name()) {
                throw std::invalid_argument(
                        fmt::format("invalid wildcard in path '{}'", full_path));
            }

            if (!children_.empty()) {
                throw std::invalid_argument(fmt::format(
                        "wildcard segment '{}' conflicts with existing children in path '{}'",
                        wildcard.name, full_path));
            }
        }

        if (wildcard.name.starts_with(':')) {
            const auto plain_segments = path.substr(0, wildcard.pos);
            if (!plain_segments.empty()) {
                path_ = plain_segments;
                has_wild_child_ = true;

                children_.push_back(std::make_unique<node>());
                auto& child = *children_.back();
                ++child.priority_;
                auto pos = std::exchange(wildcard.pos, 0);
                child.insert_path(path.substr(pos), full_path, wildcard);
            } else {
                path_ = wildcard.name;
                type_ = type::param;

                if (path.size() == wildcard.name.size()) {
                    return;
                }

                children_.push_back(std::make_unique<node>());
                auto& child = *children_.back();
                ++child.priority_;
                child.insert_path(path.substr(wildcard.name.size()), full_path, unknown_wildcard);
            }
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }

    std::string path_;
    std::string indices_;
    bool has_wild_child_{false};
    type type_{type::plain};
    int priority_{0};
    std::vector<std::unique_ptr<node>> children_;

    friend class node_test_stub;
};

} // namespace http
