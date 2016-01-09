/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef CAPTURE_STACK_TRACE_BASIC_MACROS_H_
#define CAPTURE_STACK_TRACE_BASIC_MACROS_H_

#define DISALLOW_COPY(CLASSNAME)                        \
    CLASSNAME(const CLASSNAME&) = delete;               \
    CLASSNAME& operator=(const CLASSNAME&) = delete

#define DISALLOW_MOVE(CLASSNAME)                        \
    CLASSNAME(CLASSNAME&&) = delete;                    \
    CLASSNAME& operator=(CLASSNAME&&) = delete

#define DEFAULT_COPY(classname) \
    classname(const classname&) = default; \
    classname& operator=(const classname&) = default

#define DEFAULT_MOVE(classname) \
    classname(classname&&) = default; \
    classname& operator=(classname&&) = default

#define UNREFED_VAR(x) \
    ::kbase::internal::SilenceUnusedVariableWarning(x)

// Put complicated implementation below.

namespace kbase {

namespace internal {

template<typename T>
void SilenceUnusedVariableWarning(T&&)
{}

}   // namespace internal

}   // namespace kbase

#endif  // CAPTURE_STACK_TRACE_BASIC_MACROS_H_