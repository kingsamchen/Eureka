/*
 @ 0xCCCCCCCC
*/

#include "stack_walker.h"

#include <sstream>

#include <Windows.h>

// Prevent stack frames getting messed by FPO.
#pragma optimize("", off)

StackWalker::StackWalker()
{
    auto captured_frame_count = RtlCaptureStackBackTrace(0,
                                                         static_cast<DWORD>(stack_frames_.size()),
                                                         stack_frames_.data(),
                                                         nullptr);
    valid_frame_count_ = captured_frame_count;
}

#pragma optimize("", on)
