//
// Kingsley Chen
//

#pragma once

#include <fmt/format.h>

namespace cpo2 {

template<typename T>
void fallback(T&& /*t*/) {
    fmt::println("unknown for this type");
}

namespace detail {

struct print_impl {
    template<typename I>
    void operator()(I&& impl) const {
        if constexpr (requires { std::forward<I>(impl).print(); }) {
            std::forward<I>(impl).print();
        } else {
            fallback(std::forward<I>(impl));
        }
    }
};

} // namespace detail

inline constexpr detail::print_impl print{};

} // namespace cpo2
