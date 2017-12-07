/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <type_traits>

namespace enum_ops {

template<typename E>
constexpr auto enum_cast(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

template<typename E>
constexpr std::enable_if_t<std::is_enum_v<E>, E> operator|(E lhs, E rhs) noexcept
{
    return E(enum_cast(lhs) | enum_cast(rhs));
}

template<typename E>
constexpr std::enable_if_t<std::is_enum_v<E>, E&> operator|=(E& lhs, E rhs) noexcept
{
    lhs = E(enum_cast(lhs) | enum_cast(rhs));
    return lhs;
}

template<typename E>
constexpr std::enable_if_t<std::is_enum_v<E>, E> operator&(E lhs, E rhs) noexcept
{
    return E(enum_cast(lhs) & enum_cast(rhs));
}

template<typename E>
constexpr std::enable_if_t<std::is_enum_v<E>, E&> operator&=(E& lhs, E rhs) noexcept
{
    lhs = E(enum_cast(lhs) & enum_cast(rhs));
    return lhs;
}

template<typename E>
constexpr std::enable_if_t<std::is_enum_v<E>, E> operator^(E lhs, E rhs) noexcept
{
    return E(enum_cast(lhs) ^ enum_cast(rhs));
}

template<typename E>
constexpr std::enable_if_t<std::is_enum_v<E>, E&> operator^=(E& lhs, E rhs) noexcept
{
    lhs = E(enum_cast(lhs) ^ enum_cast(rhs));
    return lhs;
}

template<typename E>
constexpr std::enable_if_t<std::is_enum_v<E>, E> operator~(E op) noexcept
{
    return E(~enum_cast(op));
}

}   // namespace enum_ops
