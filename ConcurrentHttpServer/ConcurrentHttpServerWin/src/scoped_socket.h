/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef SCOPED_SOCKET_H_
#define SCOPED_SOCKET_H_

#include <winsock2.h>

#include "kbase/scoped_handle.h"

struct SocketHandleTraits {
    using Handle = SOCKET;

    SocketHandleTraits() = delete;
    ~SocketHandleTraits() = delete;

    static Handle NullHandle() noexcept
    {
        return INVALID_SOCKET;
    }

    static bool IsValid(Handle handle) noexcept
    {
        return handle != 0 && handle != INVALID_SOCKET;
    }

    static void Close(Handle handle) noexcept
    {
        closesocket(handle);
    }
};

using ScopedSocketHandle = kbase::GenericScopedHandle<SocketHandleTraits>;

#endif  // SCOPED_SOCKET_H_
