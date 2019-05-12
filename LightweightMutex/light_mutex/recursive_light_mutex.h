/*
 @ 0xCCCCCCCC
*/

#ifndef RECURSIVE_LIGHT_MUTEX_H_
#define RECURSIVE_LIGHT_MUTEX_H_

#include <cassert>
#include <memory>

#include <Windows.h>

#include "light_mutex/common.h"

class RecursiveLightMutex {
public:
    RecursiveLightMutex()
        : sem_(MakeBenaphore()),
          owner_tid_(0),
          use_count_(0),
          recursion_count_(0)
    {
        assert(!!sem_);
    }

    ~RecursiveLightMutex()
    {}

    RecursiveLightMutex(const RecursiveLightMutex&) = delete;

    RecursiveLightMutex(RecursiveLightMutex&&) = delete;

    RecursiveLightMutex& operator=(const RecursiveLightMutex&) = delete;

    RecursiveLightMutex& operator=(RecursiveLightMutex&&) = delete;

    void lock()
    {
        auto tid = GetCurrentThreadId();
        if (_InterlockedExchangeAdd(&use_count_, 1) > 0) {
            if (owner_tid_ != tid) {
                WaitForSingleObject(sem_.get(), INFINITE);
            }
        }

        // Gained critical section guard.

        owner_tid_ = GetCurrentThreadId();
        ++recursion_count_;
    }

    bool try_lock()
    {
        auto tid = GetCurrentThreadId();
        if (owner_tid_ == tid) {
            // Already have gained the lock.
            _InterlockedExchangeAdd(&use_count_, 1);
        } else {
            if (_InterlockedCompareExchange(&use_count_, 1, 0) != 0) {
                return false;
            }
            // Gain the guard.
            owner_tid_ = tid;
        }

        ++recursion_count_;

        return true;
    }

    void unlock()
    {
        auto tid = GetCurrentThreadId();
        assert(owner_tid_ == tid);

        auto new_recur = --recursion_count_;
        if (new_recur == 0) {
            owner_tid_ = 0;
        }

        // About to lose critical section guard.
        // Read on `new_recur` instead of `recursion_count_`.
        if (_InterlockedExchangeAdd(&use_count_, -1) > 1) {
            if (new_recur == 0) {
                ReleaseSemaphore(sem_.get(), 1, nullptr);
            }
        }
    }

private:
    Handle sem_;
    DWORD owner_tid_;
    long use_count_;
    long recursion_count_;
};

#endif  // RECURSIVE_LIGHT_MUTEX_H_