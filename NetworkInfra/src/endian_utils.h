/*
 @ 0xCCCCCCCC
*/

#ifndef ENDIAN_UTILS_H_
#define ENDIAN_UTILS_H_

#include <cstdint>

#include <endian.h>

namespace network {

inline int16_t HostToNetwork(int16_t n) noexcept
{
    return htobe16(n);
}

inline uint16_t HostToNetwork(uint16_t n) noexcept
{
    return htobe16(n);
}

inline int32_t HostToNetwork(int32_t n) noexcept
{
    return htobe32(n);
}

inline uint32_t HostToNetwork(uint32_t n) noexcept
{
    return htobe32(n);
}

inline int64_t HostToNetwork(int64_t n) noexcept
{
    return htobe64(n);
}

inline uint64_t HostToNetwork(uint64_t n) noexcept
{
    return htobe64(n);
}

inline int16_t NetworkToHost(int16_t n) noexcept
{
    return be16toh(n);
}

inline uint16_t NetworkToHost(uint16_t n) noexcept
{
    return be16toh(n);
}

inline int32_t NetworkToHost(int32_t n) noexcept
{
    return be32toh(n);
}

inline uint32_t NetworkToHost(uint32_t n) noexcept
{
    return be32toh(n);
}

inline int64_t NetworkToHost(int64_t n) noexcept
{
    return be64toh(n);
}

inline uint64_t NetworkToHost(uint64_t n) noexcept
{
    return be64toh(n);
}

}   // namespace network

#endif  // ENDIAN_UTILS_H_
