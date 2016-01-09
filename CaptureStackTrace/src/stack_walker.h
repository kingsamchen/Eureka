/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef CAPTURE_STACK_TRACE_STACK_WALKER_H_
#define CAPTURE_STACK_TRACE_STACK_WALKER_H_

#include <array>

#include "basic_macros.h"

class StackWalker {
public:
    // Captures stack frame on calling thread.
    StackWalker();

    ~StackWalker() = default;

    DEFAULT_COPY(StackWalker);

    DEFAULT_MOVE(StackWalker);

private:
    static const size_t kMaxStackFrames = 64;
    std::array<void*, kMaxStackFrames> stack_frames_;
    int valid_frame_count_ = 0;
};

#endif  // CAPTURE_STACK_TRACE_STACK_WALKER_H_
