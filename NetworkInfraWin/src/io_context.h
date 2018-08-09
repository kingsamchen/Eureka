/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef IO_CONTEXT_H_
#define IO_CONTEXT_H_

#include <Windows.h>

namespace network {

enum class IOEvent {
    ReadProbe,
    Read,
    Write
};

struct IOContext : OVERLAPPED {
    IOEvent io_event;
    DWORD bytes_transferred;

    explicit IOContext(IOEvent event)
        : io_event(event),
          bytes_transferred(0)
    {
        Reset();
    }

    void Reset()
    {
        bytes_transferred = 0;
        Internal = 0;
        InternalHigh = 0;
        Offset = 0;
        OffsetHigh = 0;
        hEvent = nullptr;
    }
};

}   // namespace network

#endif  // IO_CONTEXT_H_
