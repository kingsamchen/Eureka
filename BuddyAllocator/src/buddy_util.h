/*
 @ Kingsley Chen
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef BUDDY_UTIL_H_
#define BUDDY_UTIL_H_

namespace buddy_util {

inline bool IsPowerOf2(size_t number)
{
    return number && !(number & (number - 1));
}

// number must be greater than 0.
// No internal checking for number's positiveness.
inline size_t NearestUpperPowerOf2(size_t number)
{
    --number;
    size_t half_bit_space = sizeof(size_t) << 2;
    for (size_t i = 1; i != half_bit_space; i <<= 1) {
        number |= number >> i;
    }

    return ++number;
}

// Root node is at index 0.

inline size_t Parent(size_t i)
{
    return ((i + 1) >> 1) - 1;
}

inline size_t LeftChild(size_t i)
{
    return ((i + 1) << 1) - 1;
}

inline size_t RightChild(size_t i)
{
    return (i + 1) << 1;
}

}   // namespace buddy_util

#endif  // BUDDY_UTIL_H_