/*
 @ 0xCCCCCCCC
*/

#include "socket_address.h"

#include <cstring>

#include <arpa/inet.h>

#include "kbase/error_exception_util.h"
#include "kbase/string_format.h"

namespace network {

SocketAddress::SocketAddress(const sockaddr_in& sockaddr)
    : addr_(sockaddr)
{}

SocketAddress::SocketAddress(unsigned short port)
{
    memset(&addr_, 0, sizeof(addr_));

    addr_.sin_family = AF_INET;
    addr_.sin_port = HostToNetwork(port);
    addr_.sin_addr.s_addr = INADDR_ANY;
}

SocketAddress::SocketAddress(const char* ip, unsigned short port)
{
    ENSURE(CHECK, ip != nullptr).Require();

    memset(&addr_, 0, sizeof(addr_));

    addr_.sin_family = AF_INET;
    addr_.sin_port = HostToNetwork(port);
    int rv = inet_pton(AF_INET, ip, &addr_.sin_addr);
    ENSURE(CHECK, rv > 0)(rv)(errno).Require();
}

std::string SocketAddress::ToHostPort() const
{
    constexpr socklen_t kBufSize = 16;
    char ip[kBufSize];
    auto ptr = inet_ntop(AF_INET, &addr_.sin_addr, ip, kBufSize);
    ENSURE(CHECK, ptr != nullptr)(errno).Require();

    return kbase::StringFormat("{0}:{1}", ip, port());
}

}   // namespace network
