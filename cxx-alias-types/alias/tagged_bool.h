/*
 @ 0xCCCCCCCC
*/

#ifndef ALIAS_TAGGED_BOOL_H_
#define ALIAS_TAGGED_BOOL_H_

#include <type_traits>

namespace alias {

template<typename T>
class TaggedBool {
public:
    // Allow implicit conversion from bool
    constexpr TaggedBool(bool value)
        : value_(value)
    {}

    template<typename U,
             typename=std::enable_if_t<
                 std::is_integral_v<U> && !std::is_same_v<U, bool> ||
                 std::is_floating_point_v<U> ||
                 std::is_pointer_v<U> || std::is_null_pointer_v<U>
             >>
    constexpr TaggedBool(U) = delete;

    template<typename Tag>
    constexpr explicit TaggedBool(TaggedBool<Tag> other)
        : value_(other.value_)
    {}

    // Default copy behavior

    TaggedBool(const TaggedBool&) = default;

    TaggedBool& operator=(const TaggedBool&) = default;

    // Default move behavior

    TaggedBool(TaggedBool&&) = default;

    TaggedBool& operator=(TaggedBool&&) = default;

    constexpr explicit operator bool() const noexcept
    {
        return value_;
    }

private:
    template<typename>
    friend class TaggedBool;

private:
    bool value_;
};

template<typename T>
constexpr bool operator==(TaggedBool<T> lhs, TaggedBool<T> rhs)
{
    return static_cast<bool>(lhs) == static_cast<bool>(rhs);
}

}   // namespace alias

#endif  // ALIAS_TAGGED_BOOL_H_
