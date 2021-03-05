#ifndef UUID_ENDIAN_UTILS_H_
#define UUID_ENDIAN_UTILS_H_

#include <cstdint>
#include <cstdlib>

#if defined(__linux__)
#include <endian.h>
#endif

namespace uuid {

// -*- little endian to big endian -*-

#if defined(__linux__)

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

#elif defined(_WIN32) || defined(_WIN64)

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

#endif  // OS_POSIX

// -*- big endian to little endian -*-

#if defined(__linux__)

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

#elif defined(_WIN32) || defined(_WIN64)

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

#endif

}   // namespace uuid

#endif  // UUID_ENDIAN_UTILS_H_
