/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef WINSOCK_CTX_H_
#define WINSOCK_CTX_H_

#include <WinSock2.h>
#include <MSWSock.h>

namespace winsock_ctx {

extern LPFN_ACCEPTEX AcceptEx;
extern LPFN_DISCONNECTEX DisconnectEx;

void Init();

void Cleanup();

}   // namespace winsock_ctx

#endif  // WINSOCK_CTX_H_
