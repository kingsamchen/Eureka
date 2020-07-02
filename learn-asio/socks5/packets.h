/*
 @ 0xCCCCCCCC
*/

#ifndef LEARN_ASIO_SOCKS5_PACKETS_H_
#define LEARN_ASIO_SOCKS5_PACKETS_H_

#include <array>
#include <cstdint>
#include <string>

enum class GreetingParseStage {
    Preamble,
    Methods
};

struct GreetingPacket {
    uint8_t ver {};
    uint8_t num_methods {};
    std::array<uint8_t, 255> methods {};
};

enum class RequestParseStage {
    Preamble,
    DestAddress,
    DestPort,
    Complete
};

struct RequestPacket {
    uint8_t ver {};
    uint8_t cmd {};
    uint8_t reserved {};
    uint8_t addr_type {};
    std::string dest_addr;
    uint16_t dest_port {};
};

#endif  // LEARN_ASIO_SOCKS5_PACKETS_H_
