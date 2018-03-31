/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <iterator>
#include <type_traits>

#include "array_result.h"

constexpr bool compile_time_string_equals(const char* lhs, const char* rhs)
{
    for (size_t i = 0; lhs[i] != '\0'; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

constexpr bool compile_time_string_less(const char* lhs, const char* rhs)
{
    for (size_t i = 0; lhs[i] != '\0'; ++i) {
        if (lhs[i] != rhs[i]) {
            return lhs[i] < rhs[i];
        }
    }

    return false;
}

template<typename Itr, typename Cmp>
constexpr void compile_time_sort(Itr b, Itr e, Cmp cmp)
{
    static_assert (std::is_same<bool, decltype (cmp(*b, *e))>::value, "cmp not valid.");

    using T = typename std::iterator_traits<Itr>::value_type;

    if (b == e) {
        return;
    }

    for (Itr i = b + 1; i != e; ++i) {
        Itr j = i;
        const T temp = i[0];
        for (j = i;
             (j != b) && cmp(temp, j[-1]); --j) {
            j[0] = j[-1];
        }
        j[0] = temp;
    }
}

template<std::size_t N>
constexpr auto compile_time_str_sort(const char* const(&in)[N]) -> array_result<const char*, N>
{
    array_result<const char*, N> out;

    // Copy from in to out
    for (std::size_t i = 0; i < N; ++i) {
        out[i] = in[i];
    }

    // Sort
    compile_time_sort(out.begin(), out.end(), compile_time_string_less);

    return out;
}
