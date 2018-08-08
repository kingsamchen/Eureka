/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef SCOPED_SOCKET_H_
#define SCOPED_SOCKET_H_

#include <WinSock2.h>

#include "kbase/scoped_handle.h"

namespace network {

struct SocketTraits {
    using Handle = SOCKET;

    SocketTraits() = delete;
    ~SocketTraits() = delete;

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

using ScopedSocket = kbase::GenericScopedHandle<SocketTraits>;

}   // namespace network

#endif  // SCOPED_SOCKET_H_
