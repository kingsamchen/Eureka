/*
 @ 0xCCCCCCCC
*/

#include "winsock_context.h"

#include "kbase/error_exception_util.h"

#include "scoped_socket.h"

namespace {

network::WinsockContext* ctx_instance = nullptr;

template<typename P>
void GetExtensionFunctionPointer(SOCKET sock, P& pfn, GUID guid)
{
    DWORD recv_bytes = 0;
    auto rv = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &pfn,
                       sizeof(pfn), &recv_bytes, nullptr, nullptr);
    ENSURE(CHECK, rv == 0)(rv)(WSAGetLastError()).Require();
}

}   // namespace

namespace network {

WinsockContext::WinsockContext()
    : AcceptEx(nullptr),
      DisconnectEx(nullptr)
{
    ENSURE(CHECK, ctx_instance == nullptr).Require();
    ctx_instance = this;

    WSADATA data {0};
    auto result_code = WSAStartup(MAKEWORD(2, 2), &data);
    ENSURE(THROW, result_code == 0)(result_code).Require();

    ScopedSocket sock(socket(AF_INET, SOCK_STREAM, 0));
    ENSURE(CHECK, !!sock)(WSAGetLastError()).Require();

    GetExtensionFunctionPointer(sock.get(), AcceptEx, WSAID_ACCEPTEX);
    GetExtensionFunctionPointer(sock.get(), DisconnectEx, WSAID_DISCONNECTEX);
}

WinsockContext::~WinsockContext()
{
    WSACleanup();
    ctx_instance = nullptr;
}

// static
const WinsockContext& WinsockContext::instance()
{
    ENSURE(CHECK, ctx_instance != nullptr).Require();
    return *ctx_instance;
}

}   // namespace network
