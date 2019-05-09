/*
 @ 0xCCCCCCCC
*/

#ifndef LIGHT_MUTEX_H_
#define LIGHT_MUTEX_H_

#include <cassert>
#include <memory>

#include <Windows.h>
#include <intrin.h>

using Handle = std::unique_ptr<void, decltype(&CloseHandle)>;

inline Handle MakeBenaphore()
{
    return Handle(CreateSemaphoreW(nullptr, 0, 1, nullptr), CloseHandle);
}

class LightMutex {
public:
    LightMutex()
        : sem_(MakeBenaphore()),
          use_count_(0)
    {
        assert(!!sem_);
    }

    ~LightMutex()
    {}

    LightMutex(const LightMutex&) = delete;

    LightMutex(LightMutex&&) = delete;

    LightMutex& operator=(const LightMutex&) = delete;

    LightMutex& operator=(LightMutex&&) = delete;

    void lock()
    {
        // This intrinsic returns original value.
        if (_InterlockedExchangeAdd(&use_count_, 1) > 0) {
            WaitForSingleObject(sem_.get(), INFINITE);
        }
    }

    bool try_lock()
    {
        return _InterlockedCompareExchange(&use_count_, 1, 0) == 0;
    }

    void unlock()
    {
        if (_InterlockedExchangeAdd(&use_count_, -1) > 1) {
            ReleaseSemaphore(sem_.get(), 1, nullptr);
        }
    }

private:
    Handle sem_;
    long use_count_;
};

#endif  // LIGHT_MUTEX_H_