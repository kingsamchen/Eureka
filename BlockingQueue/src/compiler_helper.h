/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef COMPILER_HELPER_H_
#define COMPILER_HELPER_H_

#define DISALLOW_COPY(CLASS)                    \
    CLASS(const CLASS&) = delete;               \
    CLASS& operator=(const CLASS&) = delete;

#define DISALLOW_MOVE(CLASS)                    \
    CLASS(CLASS&&) = delete;                    \
    CLASS& operator=(CLASS&&) = delete;

#endif  // COMPILER_HELPER_H_