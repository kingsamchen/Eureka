/*
 @ 0xCCCCCCCC
*/

#ifndef LEARN_ASIO_SOCKS5_PACKETS_H_
#define LEARN_ASIO_SOCKS5_PACKETS_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

enum class GreetingParseStage {
    Preamble,
    Methods
};

struct GreetingPacket {
    std::byte ver {};
    std::byte num_methods {};
    std::array<std::byte, 255> methods {};
};

enum class RequestParseStage {
    Preamble,
    DestAddress,
    DestPort,
    Complete
};

struct RequestPacket {
    std::byte ver {};
    std::byte cmd {};
    std::byte reserved {};
    std::byte addr_type {};
    std::string dest_addr;
    uint16_t dest_port {};
};

#endif  // LEARN_ASIO_SOCKS5_PACKETS_H_
