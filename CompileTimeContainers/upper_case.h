/*
 @ 0xCCCCCCCC
*/

#pragma once

#include "array_result.h"

template<size_t N>
constexpr auto MakeUpperCase(const char (&str)[N]) -> array_result<char, N>
{
    array_result<char, N> new_str;

    for (size_t i = 0; i < N; ++i) {
        char ch = str[i];
        new_str[i] = ((ch < 'a') || (ch > 'z')) ? ch : ch - ('a' - 'A');
    }

    return new_str;
}
