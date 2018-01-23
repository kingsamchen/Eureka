/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef IOCP_UTILS_H_
#define IOCP_UTILS_H_

#include <Windows.h>

#include "kbase/scoped_handle.h"

namespace utils {

enum CompletionKey : ULONG_PTR {
    CompletionKeyAccept = 0,
    CompletionKeyIO,
    CompletionKeyShutdown
};

inline kbase::ScopedWinHandle CreateNewIOCP(DWORD worker_thread_count)
{
    return kbase::ScopedWinHandle(
        CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, worker_thread_count));
}

inline bool AssociateDeviceWithIOCP(HANDLE device, HANDLE iocp, ULONG_PTR key)
{
    return CreateIoCompletionPort(device, iocp, key, 0) == iocp;
}

}   // namespace utils

#endif  // IOCP_UTILS_H_
