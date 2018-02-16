/*
 @ 0xCCCCCCCC
*/

#include "winsock_ctx.h"

#include <stdio.h>

#include "kbase/error_exception_util.h"

#include "scoped_socket.h"

namespace {

ScopedSocketHandle sock;

template<typename P>
void GetExtensionFunctionPointer(P& pfn, GUID guid)
{
    DWORD recv_bytes = 0;
    auto rv = WSAIoctl(sock.get(), SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &pfn,
                       sizeof(pfn), &recv_bytes, nullptr, nullptr);
    ENSURE(CHECK, rv == 0)(rv)(WSAGetLastError()).Require();
}

}   // namespace

namespace winsock_ctx {

LPFN_ACCEPTEX AcceptEx = nullptr;
LPFN_DISCONNECTEX DisconnectEx = nullptr;

void Init()
{
    WSADATA data {0};
    auto result_code = WSAStartup(MAKEWORD(2, 2), &data);
    ENSURE(THROW, result_code == 0)(result_code).Require();

    sock.reset(socket(AF_INET, SOCK_STREAM, 0));
    ENSURE(CHECK, !!sock)(WSAGetLastError()).Require();

    GetExtensionFunctionPointer(AcceptEx, WSAID_ACCEPTEX);
    GetExtensionFunctionPointer(DisconnectEx, WSAID_DISCONNECTEX);

    sock.reset();

    printf("-*- Windows Socket Library Initialized -*-\n");
}

void Cleanup()
{
    WSACleanup();

    printf("-*- Windows Socket Library Cleaned -*-\n");
}

}   // namespace winsock_ctx
