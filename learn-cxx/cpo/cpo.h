//
// Kingsley Chen
//

#include <type_traits>
#include <utility>

#include <fmt/format.h>

namespace cpo {

template<typename T>
void fallback(T&& /*t*/) {
    fmt::println("unknown for this type");
}

namespace detail {

template<typename T, typename U>
decltype(auto) tag_invoke(T, U);

template<typename T, typename U, typename = void>
struct is_tag_invocable : std::false_type {};

template<typename T, typename U>
struct is_tag_invocable<T, U,
                        std::void_t<decltype(tag_invoke(std::declval<T>(), std::declval<U>()))>>
    : std::true_type {};

template<typename T, typename U>
inline constexpr bool is_tag_invocable_v = is_tag_invocable<T, U>::value;

struct print_impl_sfinae {
    template<typename T>
    void operator()(T&& t) const {
        if constexpr (is_tag_invocable_v<print_impl_sfinae, T>) {
            tag_invoke(*this, std::forward<T>(t));
        } else {
            return fallback(std::forward<T>(t));
        }
    }
};

struct print_impl_concept {
    template<typename T>
    void operator()(T&& t) const {
        if constexpr (requires { tag_invoke(*this, std::forward<T>(t)); }) {
            tag_invoke(*this, std::forward<T>(t));
        } else {
            return fallback(std::forward<T>(t));
        }
    }
};

} // namespace detail

inline constexpr detail::print_impl_concept print_concept{};
inline constexpr detail::print_impl_sfinae print_sfinae{};

} // namespace cpo
