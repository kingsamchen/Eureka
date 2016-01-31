/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EUREKA_ACTIVE_THREAD_BASIC_MACROS_H_
#define EUREKA_ACTIVE_THREAD_BASIC_MACROS_H_

#define DISALLOW_COPY(CLASSNAME)                        \
    CLASSNAME(const CLASSNAME&) = delete;               \
    CLASSNAME& operator=(const CLASSNAME&) = delete

#define DISALLOW_MOVE(CLASSNAME)                        \
    CLASSNAME(CLASSNAME&&) = delete;                    \
    CLASSNAME& operator=(CLASSNAME&&) = delete

#define UNREFED_VAR(x) \
    ::kbase::internal::SilenceUnusedVariableWarning(x)

// Put complicated implementation below.

namespace internal {

template<typename T>
void SilenceUnusedVariableWarning(T&&)
{}

}   // namespace internal

#endif  // EUREKA_ACTIVE_THREAD_BASIC_MACROS_H_