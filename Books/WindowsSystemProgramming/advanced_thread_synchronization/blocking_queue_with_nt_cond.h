/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <queue>

#include <Windows.h>

#include "kbase/basic_macros.h"

class MTQueue {
public:
    explicit MTQueue(size_t capacity);

    ~MTQueue();

    DISALLOW_COPY(MTQueue);

    DISALLOW_MOVE(MTQueue);

    void Put(int data);

    int Pop();

private:
    bool empty() const;

    bool full() const;

private:
    CRITICAL_SECTION queue_mutex_;
    CONDITION_VARIABLE not_empty_;
    CONDITION_VARIABLE not_full_;
    size_t capacity_;
    std::queue<int> queue_;
};