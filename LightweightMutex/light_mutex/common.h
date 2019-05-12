/*
 @ 0xCCCCCCCC
*/

#ifndef COMMON_H_
#define COMMON_H_

#include <memory>

#include <Windows.h>

using Handle = std::unique_ptr<void, decltype(&CloseHandle)>;

inline Handle MakeBenaphore()
{
    return Handle(CreateSemaphoreW(nullptr, 0, 1, nullptr), CloseHandle);
}

#endif  // COMMON_H_
