//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef TOPT_ENDIAN_H_
#define TOPT_ENDIAN_H_

#include <cstdint>
#include <cstdlib>

#if !defined(_WIN32)
#include <endian.h>
#endif

namespace topt {

// -*- little endian to big endian -*-

#if !defined(_WIN32)

inline int16_t host_to_network(int16_t n) noexcept {
    return htobe16(n);
}

inline uint16_t host_to_network(uint16_t n) noexcept {
    return htobe16(n);
}

inline int32_t host_to_network(int32_t n) noexcept {
    return htobe32(n);
}

inline uint32_t host_to_network(uint32_t n) noexcept {
    return htobe32(n);
}

inline int64_t host_to_network(int64_t n) noexcept {
    return htobe64(n);
}

inline uint64_t host_to_network(uint64_t n) noexcept {
    return htobe64(n);
}

#else

inline int16_t host_to_network(int16_t n) noexcept {
    return _byteswap_ushort(n);
}

inline uint16_t host_to_network(uint16_t n) noexcept {
    return _byteswap_ushort(n);
}

inline int32_t host_to_network(int32_t n) noexcept {
    return _byteswap_ulong(n);
}

inline uint32_t host_to_network(uint32_t n) noexcept {
    return _byteswap_ulong(n);
}

inline int64_t host_to_network(int64_t n) noexcept {
    return _byteswap_uint64(n);
}

inline uint64_t host_to_network(uint64_t n) noexcept {
    return _byteswap_uint64(n);
}

#endif // !defined(_WIN32)

// -*- big endian to little endian -*-

#if !defined(_WIN32)

inline int16_t network_to_host(int16_t n) noexcept {
    return be16toh(n);
}

inline uint16_t network_to_host(uint16_t n) noexcept {
    return be16toh(n);
}

inline int32_t network_to_host(int32_t n) noexcept {
    return be32toh(n);
}

inline uint32_t network_to_host(uint32_t n) noexcept {
    return be32toh(n);
}

inline int64_t network_to_host(int64_t n) noexcept {
    return be64toh(n);
}

inline uint64_t network_to_host(uint64_t n) noexcept {
    return be64toh(n);
}

#else

inline int16_t network_to_host(int16_t n) noexcept {
    return _byteswap_ushort(n);
}

inline uint16_t network_to_host(uint16_t n) noexcept {
    return _byteswap_ushort(n);
}

inline int32_t network_to_host(int32_t n) noexcept {
    return _byteswap_ulong(n);
}

inline uint32_t network_to_host(uint32_t n) noexcept {
    return _byteswap_ulong(n);
}

inline int64_t network_to_host(int64_t n) noexcept {
    return _byteswap_uint64(n);
}

inline uint64_t network_to_host(uint64_t n) noexcept {
    return _byteswap_uint64(n);
}

#endif // !defined(_WIN32)

} // namespace topt

#endif // TOPT_ENDIAN_H_
