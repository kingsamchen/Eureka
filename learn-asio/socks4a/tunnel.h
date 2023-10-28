/*
 @ 0xCCCCCCCC
*/

#ifndef TUNNEL_H_
#define TUNNEL_H_

#include <cstdint>
#include <memory>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "fmt/format.h"

using asio::ip::tcp;

struct RequestPacket {
    uint8_t ver = 0;
    uint8_t cmd = 0;
    uint16_t dest_port = 0;
    uint32_t dest_ip = 0;
    std::string user_id;
    std::string domain;

    static constexpr size_t kPreambleSize =
            sizeof(ver) + sizeof(cmd) + sizeof(dest_port) + sizeof(dest_ip);

    static constexpr uint8_t kVer4 = 4;
    static constexpr uint8_t kCmdConnect = 1;
    static constexpr uint32_t k4aMask = 256;

    bool Valid() const noexcept {
        if (ver != kVer4 || cmd != kCmdConnect || dest_port == 0 || dest_ip == 0) {
            return false;
        }

        return !(dest_ip < k4aMask && domain.empty());
    }
};

inline std::ostream& operator<<(std::ostream& os, const RequestPacket& packet) {
    auto s = fmt::format("ver={0} cmd={1} dest-port={2} dest-ip={3} user-id={4} domain={5}",
                         +packet.ver, +packet.cmd, packet.dest_port, packet.dest_ip,
                         packet.user_id, packet.domain);
    os << s;
    return os;
}

enum class RequestParseStage {
    Preamble,
    UserId,
    Domain,
    Complete
};

class Tunnel : public std::enable_shared_from_this<Tunnel> {
public:
    Tunnel(asio::io_context& ctx, tcp::socket&& client_sock);

    ~Tunnel() = default;

    Tunnel(const Tunnel&) = delete;
    Tunnel& operator=(const Tunnel&) = delete;

    void Run();

private:
    void ReadRequestPacket(std::unique_ptr<RequestPacket> packet, RequestParseStage state);

    void ConnectRemote(std::unique_ptr<RequestPacket> packet);

    void ForwardTransmission();

    void ForwardClientRequest();

    void ForwardRemoteResponse();

    void SetNoDelay();

    void RejectClient();

    void ForceClose();

private:
    asio::io_context& io_ctx_;
    tcp::socket client_sock_;
    bool client_sock_disconnected_;
    tcp::endpoint client_addr_;
    tcp::socket dial_sock_;
    bool dial_sock_disconnected_;
    tcp::endpoint remote_addr_;
    std::string io_client_buf_;
    std::string io_remote_buf_;
};

#endif // TUNNEL_H_