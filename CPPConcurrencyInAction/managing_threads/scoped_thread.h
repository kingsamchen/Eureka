/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <thread>

#include "kbase/basic_macros.h"
#include "kbase/error_exception_util.h"

class ScopedThread {
public:
    explicit ScopedThread(std::thread&& t)
        : thread_(std::move(t))
    {
        ENSURE(THROW, thread_.joinable()).Require();
    }

    ~ScopedThread()
    {
        thread_.join();
    }

    DISALLOW_COPY(ScopedThread);

private:
    std::thread thread_;
};
