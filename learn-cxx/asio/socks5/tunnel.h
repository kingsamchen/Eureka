/*
 @ 0xCCCCCCCC
*/

#ifndef LEARN_ASIO_SOCKS5_TUNNEL_H_
#define LEARN_ASIO_SOCKS5_TUNNEL_H_

#include <memory>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"

#include "packets.h"

using asio::ip::tcp;

class Tunnel : public std::enable_shared_from_this<Tunnel> {
private:
    class Passkey {
        Passkey() noexcept {}
        friend Tunnel;
    };

    enum class RejectReason : uint8_t {
        GeneralFailure = 0x01,
        HostUnreachable = 0x04,
        CommandNotSupported = 0x07
    };

public:
    Tunnel(Passkey, asio::io_context::executor_type executor, tcp::socket&& client_sock);

    ~Tunnel();

    Tunnel(const Tunnel&) = delete;
    Tunnel& operator=(const Tunnel&) = delete;

    static std::shared_ptr<Tunnel> Make(asio::io_context::executor_type executor,
                                        tcp::socket&& client_sock);

    void Run();

private:
    void ReadGreetingPacket(std::unique_ptr<GreetingPacket> packet, GreetingParseStage stage);

    void ReplyGreeting();

    void ReadRequestpacket(std::unique_ptr<RequestPacket> packet, RequestParseStage stage);

    void ReplyRequest();

    void ConnetRemote(std::unique_ptr<RequestPacket> packet);

    void StartTunneling();

    void ForwardClientRequest();

    void ForwardRemoteResponse();

    void ShutdownClient();

    void RejectClient(RejectReason reason);

    void ForceShutdown();

private:
    asio::io_context::executor_type executor_;

    tcp::socket client_sock_;
    tcp::endpoint client_addr_;
    bool client_sock_disconnected_;

    tcp::socket dial_sock_;
    tcp::endpoint remote_addr_;
    bool dial_sock_disconnected_;

    std::string io_client_buf_;
    std::string io_remote_buf_;
};

#endif // LEARN_ASIO_SOCKS5_TUNNEL_H_
