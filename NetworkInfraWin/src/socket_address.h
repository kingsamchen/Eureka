/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef SOCKET_ADDRESS_H_
#define SOCKET_ADDRESS_H_

#include <string>

#include <WinSock2.h>

#include "kbase/basic_macros.h"

#include "endian_utils.h"

namespace network {

class SocketAddress {
public:
    explicit SocketAddress(const sockaddr_in& sockaddr);

    explicit SocketAddress(unsigned short port);

    SocketAddress(const char* ip, unsigned short port);

    DEFAULT_COPY(SocketAddress);

    DEFAULT_MOVE(SocketAddress);

    unsigned short port() const noexcept
    {
        return NetworkToHost(addr_.sin_port);
    }

    const sockaddr_in& raw() const noexcept
    {
        return addr_;
    }

    std::string ToHostPort() const;

private:
    sockaddr_in addr_;
};

}   // namespace network

#endif  // SOCKET_ADDRESS_H_
