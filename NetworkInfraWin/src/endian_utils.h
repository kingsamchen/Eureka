/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef ENDIAN_UTILS_H_
#define ENDIAN_UTILS_H_

#include <cstdint>
#include <cstdlib>

namespace network {

inline int16_t HostToNetwork(int16_t n) noexcept
{
    return _byteswap_ushort(n);
}

inline uint16_t HostToNetwork(uint16_t n) noexcept
{
    return _byteswap_ushort(n);
}

inline int32_t HostToNetwork(int32_t n) noexcept
{
    return _byteswap_ulong(n);
}

inline uint32_t HostToNetwork(uint32_t n) noexcept
{
    return _byteswap_ulong(n);
}

inline int64_t HostToNetwork(int64_t n) noexcept
{
    return _byteswap_uint64(n);
}

inline uint64_t HostToNetwork(uint64_t n) noexcept
{
    return _byteswap_uint64(n);
}

inline int16_t NetworkToHost(int16_t n) noexcept
{
    return _byteswap_ushort(n);
}

inline uint16_t NetworkToHost(uint16_t n) noexcept
{
    return _byteswap_ushort(n);
}

inline int32_t NetworkToHost(int32_t n) noexcept
{
    return _byteswap_ulong(n);
}

inline uint32_t NetworkToHost(uint32_t n) noexcept
{
    return _byteswap_ulong(n);
}

inline int64_t NetworkToHost(int64_t n) noexcept
{
    return _byteswap_uint64(n);
}

inline uint64_t NetworkToHost(uint64_t n) noexcept
{
    return _byteswap_uint64(n);
}

}   // namespace network

#endif  // ENDIAN_UTILS_H_
