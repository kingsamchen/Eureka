// Kingsley Chen

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <esl/ignore_unused.h>
#include <esl/strings.h>
#include <fmt/format.h>

namespace http {

struct request {};
struct response {};

using handler_t = std::function<void(request, response)>;

// TODO(KC): put into esl.
template<typename E>
constexpr std::underlying_type_t<E> to_underlying(E e) {
    return static_cast<std::underlying_type_t<E>>(e);
}

struct param {
    std::string_view key;
    std::string_view value;

    friend bool operator==(param lhs, param rhs) = default;
};

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

// Valid since C++20.
static_assert(std::is_aggregate_v<wildcard_result>);

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
    void add_route(std::string_view path, handler_t handler) {
        // The sub-tree rooted by the node has one more route.
        ++priority_;

        auto full_path = path;
        if (path_.empty() && indices_.empty()) {
            insert_path(path, full_path, unknown_wildcard, std::move(handler));
            type_ = type::root;
            return;
        }

        insert_route(path, full_path, std::move(handler));
    }

    // TODO(KC): make the locate const-ness? how to resolve non-const handler issue?
    handler_t* locate(std::string_view path, std::vector<param>& params) {
        if (path.size() == path_.size()) {
            return handler_ ? &handler_ : nullptr;
        }

        if (path.size() > path_.size() && path.starts_with(path_)) {
            path.remove_prefix(path_.size());

            if (!has_wild_child_) {
                const char idxc = path[0];
                const auto pos = indices_.find(idxc);
                if (pos == std::string::npos) {
                    return nullptr;
                }

                return children_[pos]->locate(path, params);
            }

            // TODO(KC): restrict dynamic allocation for params???
            // assert(params.size() < params.capacity());
            auto& child = *children_.front();
            if (child.type_ == type::param) {
                const auto param_end = path.find('/');

                params.push_back(param{.key = std::string_view{child.path_}.substr(1),
                                       .value = path.substr(0, param_end)});

                if (param_end == std::string_view::npos) {
                    return child.handler_ ? &child.handler_ : nullptr;
                }

                // Go deeper.
                if (!child.children_.empty()) {
                    return child.children_[0]->locate(path.substr(param_end), params);
                }
            } else if (child.type_ == type::catch_all) {
                params.push_back(param{.key = std::string_view{child.path_}.substr(2),
                                       .value = path});
                return &child.handler_;
            } else [[unlikely]] {
                throw std::runtime_error(fmt::format("node type '{}' of route '{}' is invalid",
                                                     to_underlying(child.type_), child.path_));
            }
        }

        return nullptr;
    }

private:
    void insert_route(std::string_view path, std::string_view full_path, handler_t handler) {
        const auto len = longest_common_prefix(path, path_);

        // Split current node to make node path equal to common prefix.
        if (len < path_.size()) {
            auto child = std::make_unique<node>();
            child->path_ = path_.substr(len);
            child->indices_ = indices_;
            child->has_wild_child_ = has_wild_child_;
            child->type_ = type::plain;
            child->priority_ = priority_ - 1;
            child->children_ = std::move(children_);
            child->handler_ = std::move(handler_);

            children_.clear(); // Reset to known state for reuse.
            children_.push_back(std::move(child));
            indices_ = path_[len];
            path_.erase(len);
            has_wild_child_ = false;
        }

        // `path` is subset of a route path denoted by current node.
        if (len == path.size()) {
            if (handler_) {
                throw std::invalid_argument(
                        fmt::format("a handler is already registered for path '{}'", full_path));
            }
            handler_ = std::move(handler);
            return;
        }

        path.remove_prefix(len);

        // Can have only one child if it is a wild child.
        if (has_wild_child_) {
            auto& child = *children_.front();
            ++child.priority_;
            if (path.starts_with(child.path_) &&
                child.type_ != type::catch_all &&
                (child.path_.size() == path.size() || path[child.path_.size()] == '/')) {
                child.insert_route(path, full_path, std::move(handler));
                return;
            }

            // Wildcard conflict.
            const auto segment = child.type_ != type::catch_all
                                         ? *esl::strings::split(path, '/').begin()
                                         : path;
            std::string prefix{full_path.substr(0, full_path.find(segment))};
            prefix += child.path_;
            throw std::invalid_argument(
                    fmt::format("'{}' in path '{}' conflicts with existing wildcard '{}' in '{}'",
                                segment, full_path, child.path_, prefix));
        }

        auto idxc = path[0];

        // If current node is a param then its `indices_` is always empty, and the node can have
        // at most one child, and the path of this child either is `/` or starts with `/`.
        if (type_ == type::param && idxc == '/' && !children_.empty()) {
            assert(children_.size() == 1);
            auto& child = *children_.front();
            ++child.priority_;
            child.insert_route(path, full_path, std::move(handler));
            return;
        }

        // Check if a child with the next path character exists.
        for (std::size_t i = 0; i < indices_.size(); ++i) {
            if (auto c = indices_[i]; c == idxc) {
                i = increment_child_priority(i);
                children_[i]->insert_route(path, full_path, std::move(handler));
                return;
            }
        }

        if (idxc != ':' && idxc != '*') {
            indices_ += idxc;
            children_.push_back(std::make_unique<node>());
            auto& child = *children_.back();
            esl::ignore_unused(increment_child_priority(indices_.size() - 1));
            child.insert_path(path, full_path, unknown_wildcard, std::move(handler));
            return;
        }

        insert_path(path, full_path, unknown_wildcard, std::move(handler));
    }

    void insert_path(std::string_view path, std::string_view full_path, wildcard_result wildcard,
                     handler_t handler) {
        // The invocation with no wildcard means we haven't scanned the `path` yet.
        // Let's scan the path in flight.
        if (!wildcard.found()) {
            wildcard = find_wildcard(path);
            if (!wildcard.found()) {
                path_ = path;
                handler_ = std::move(handler);
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
                path.remove_prefix(plain_segments.size());
            }

            has_wild_child_ = true;

            // The param node.
            children_.push_back(std::make_unique<node>());
            auto& child = *children_.back();
            child.priority_ = 1;
            child.type_ = type::param;
            child.path_ = wildcard.name;

            // The path ends with the wildcard, the param node is the leaf.
            if (path.size() == wildcard.name.size()) {
                child.handler_ = std::move(handler);
                return;
            }

            // There are another non-wildcard subpath.
            child.children_.push_back(std::make_unique<node>());
            auto& grand_child = *child.children_.back();
            grand_child.priority_ = 1;
            grand_child.insert_path(path.substr(wildcard.name.size()), full_path, unknown_wildcard,
                                    std::move(handler));
        } else {
            if (wildcard.pos + wildcard.name.size() != path.size()) {
                throw std::invalid_argument(fmt::format(
                        "catch-all is only allowed at the end of the path in '{}'", full_path));
            }

            // e.g. `/hello/*name` would conflict with `/hello/` but not `/hello`.
            if (!path_.empty() && path_.ends_with('/')) {
                throw std::invalid_argument(fmt::format(
                        "catch-all conflicts with existing handle for path segment root in '{}'",
                        full_path));
            }

            // Move to leading `/`.
            assert(wildcard.pos > 0);
            if (path[--wildcard.pos] != '/') {
                throw std::invalid_argument(fmt::format(
                        "no / before catch-all in path '{}'", full_path));
            }

            path_ = path.substr(0, wildcard.pos);

            // First node is a catch-all node but with empty path.
            auto child = std::make_unique<node>();
            child->has_wild_child_ = true;
            child->type_ = type::catch_all;
            child->priority_ = 1;

            // Second node is a catch-all node holding the variable.
            auto grand_child = std::make_unique<node>();
            grand_child->path_ = path.substr(wildcard.pos);
            grand_child->type_ = type::catch_all;
            grand_child->priority_ = 1;
            grand_child->handler_ = std::move(handler);

            child->children_.push_back(std::move(grand_child));
            children_.push_back(std::move(child));
            indices_ = '/';
        }
    }

    // Increments priority of the given child and reorders if necessary.
    std::size_t increment_child_priority(std::size_t pos) {
        assert(indices_.size() == children_.size());

        const auto prio = ++(children_[pos]->priority_);

        std::size_t new_pos = pos;
        for (; new_pos > 0 && children_[new_pos - 1]->priority_ < prio; --new_pos) {
            ;
        }

        if (new_pos != pos) {
            const auto begin_difft = static_cast<std::ptrdiff_t>(new_pos);
            const auto end_difft = static_cast<std::ptrdiff_t>(pos + 1);

            auto child = std::move(children_[pos]);
            std::shift_right(std::next(children_.begin(), begin_difft),
                             std::next(children_.begin(), end_difft),
                             1);
            children_[new_pos] = std::move(child);

            const auto idxc = indices_[pos];
            std::shift_right(std::next(indices_.begin(), begin_difft),
                             std::next(indices_.begin(), end_difft),
                             1);
            indices_[new_pos] = idxc;
        }

        return new_pos;
    }

    std::string path_;
    std::string indices_;
    bool has_wild_child_{false};
    type type_{type::plain};
    int priority_{0};
    std::vector<std::unique_ptr<node>> children_;
    handler_t handler_;

    friend class node_test_stub;
};

} // namespace http
