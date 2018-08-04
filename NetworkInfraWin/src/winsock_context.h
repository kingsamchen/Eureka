/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef WINSOCK_CONTEXT_H_
#define WINSOCK_CONTEXT_H_

#include <WinSock2.h>
#include <MSWSock.h>

namespace network {

struct WinsockContext {
    WinsockContext();

    ~WinsockContext();

    static const WinsockContext& instance();

    LPFN_ACCEPTEX AcceptEx;
    LPFN_DISCONNECTEX DisconnectEx;
};

}   // namespace network

#endif  // WINSOCK_CONTEXT_H_
