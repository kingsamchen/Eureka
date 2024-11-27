/*
 @ 0xCCCCCCCC
*/

#include "tunnel.h"

#include <cassert>
#include <cstddef>
#include <functional>
#include <string_view>

#include "asio/connect.hpp"
#include "asio/read.hpp"
#include "asio/write.hpp"

#include "esl/byteswap.h"
#include "glog/logging.h"

using namespace std::placeholders;

namespace {

constexpr std::byte kSocks5Ver{0x05};
constexpr std::byte kCmdConnect{0x01};
constexpr std::byte kAddrIPv4{0x01};
constexpr std::byte kAddrDomain{0x03};
constexpr std::byte kAddrIPv6{0x04};

void ShutdownWriteSafe(tcp::socket& sock, bool flag) {
    if (flag) {
        return;
    }

    std::error_code ec;
    sock.shutdown(tcp::socket::shutdown_send, ec);
    LOG_IF(WARNING, !!ec) << "Failed to shutdown send on sock @ " << sock.remote_endpoint()
                          << " ec=" << ec;
}

} // namespace

Tunnel::Tunnel(Passkey, asio::io_context::executor_type executor, tcp::socket&& client_sock)
    : executor_(executor),
      client_sock_(std::move(client_sock)),
      client_addr_(client_sock_.remote_endpoint()),
      client_sock_disconnected_(false),
      dial_sock_(executor),
      dial_sock_disconnected_(false) {}

Tunnel::~Tunnel() {
    DLOG(INFO) << "Tunnel from " << client_addr_ << " died";
}

// static
std::shared_ptr<Tunnel> Tunnel::Make(asio::io_context::executor_type executor,
                                     tcp::socket&& client_sock) {
    return std::make_shared<Tunnel>(Passkey{}, executor, std::move(client_sock));
}

void Tunnel::Run() {
    LOG(INFO) << "Tunnel is running for client from " << client_sock_.remote_endpoint();
    ReadGreetingPacket(std::make_unique<GreetingPacket>(), GreetingParseStage::Preamble);
}

void Tunnel::ReadGreetingPacket(std::unique_ptr<GreetingPacket> packet, GreetingParseStage stage) {
    constexpr size_t kPreambleBytes = 2;

    if (stage == GreetingParseStage::Preamble) {
        std::array<asio::mutable_buffer, kPreambleBytes> buf{
                asio::buffer(&packet->ver, sizeof(packet->ver)),
                asio::buffer(&packet->num_methods, sizeof(packet->num_methods))};

        asio::async_read(
                client_sock_,
                buf,
                [this, self = shared_from_this(), pkt = std::move(packet)](auto ec, auto) mutable {
                    if (ec) {
                        LOG(ERROR) << "Failed to read greeting preamble; ec=" << ec;
                        ShutdownClient();
                        return;
                    }

                    if (pkt->ver != kSocks5Ver || pkt->num_methods == std::byte(0)) {
                        LOG(WARNING) << "Failed to parse greeting preamble; incorrect data received.";
                        ShutdownClient();
                        return;
                    }

                    ReadGreetingPacket(std::move(pkt), GreetingParseStage::Methods);
                });
    } else {
        auto buf = asio::buffer(packet->methods.data(), std::to_integer<size_t>(packet->num_methods));
        asio::async_read(
                client_sock_,
                buf,
                [this, self = shared_from_this(), pkt = std::move(packet)](auto ec, auto) mutable {
                    if (ec) {
                        LOG(ERROR) << "Failed to read greeting methods; ec=" << ec;
                        ShutdownClient();
                        return;
                    }

                    // Our proxy always and only supports 0x00 (NO AUTHENTICATION REQUIRED)
                    ReplyGreeting();
                });
    }
}

void Tunnel::ReplyGreeting() {
    using namespace std::string_view_literals;
    constexpr std::string_view kReplyGreeting = "\x05\x00"sv;
    asio::async_write(
            client_sock_,
            asio::buffer(kReplyGreeting),
            [this, self = shared_from_this()](std::error_code ec, size_t) {
                if (ec) {
                    LOG(ERROR) << "Failed to write greeting reply; ec=" << ec;
                    ShutdownClient();
                    return;
                }

                ReadRequestpacket(std::make_unique<RequestPacket>(), RequestParseStage::Preamble);
            });
}

void Tunnel::ReadRequestpacket(std::unique_ptr<RequestPacket> packet, RequestParseStage stage) {
    switch (stage) {
    case RequestParseStage::Preamble: {
        std::array buf{
                asio::buffer(&packet->ver, sizeof(packet->ver)),
                asio::buffer(&packet->cmd, sizeof(packet->cmd)),
                asio::buffer(&packet->reserved, sizeof(packet->reserved)),
                asio::buffer(&packet->addr_type, sizeof(packet->addr_type))};
        asio::async_read(
                client_sock_,
                buf,
                [this, self = shared_from_this(), pkt = std::move(packet)](auto ec, auto) mutable {
                    if (ec) {
                        LOG(ERROR) << "Failed to read request preamble; ec=" << ec;
                        ShutdownClient();
                        return;
                    }

                    if (pkt->ver != kSocks5Ver) {
                        LOG(WARNING) << "Failed to continue request due to incorrect version";
                        RejectClient(RejectReason::GeneralFailure);
                        return;
                    }

                    if (pkt->cmd != kCmdConnect) {
                        LOG(WARNING) << "Failed to continue request due to unsupported cmd";
                        RejectClient(RejectReason::CommandNotSupported);
                        return;
                    }

                    ReadRequestpacket(std::move(pkt), RequestParseStage::DestAddress);
                });
        break;
    }
    case RequestParseStage::DestAddress: {
        if (packet->addr_type == kAddrDomain) {
            packet->dest_addr.resize(1);
            auto buf = asio::buffer(packet->dest_addr.data(), 1);
            asio::async_read(
                    client_sock_,
                    buf,
                    [this, self = shared_from_this(), pkt = std::move(packet)](auto ec, auto) mutable {
                        if (ec) {
                            LOG(ERROR) << "Failed to read request dest addr; ec=" << ec;
                            RejectClient(RejectReason::GeneralFailure);
                            return;
                        }

                        size_t length = static_cast<uint8_t>(pkt->dest_addr[0]);
                        pkt->dest_addr.resize(length);
                        auto buf2 = asio::buffer(pkt->dest_addr.data(), length);
                        asio::async_read(
                                client_sock_,
                                buf2,
                                [this, self = shared_from_this(), pkt = std::move(pkt)](auto e, auto) mutable {
                                    if (e) {
                                        LOG(ERROR) << "Failed to read request dest addr; ec=" << e;
                                        RejectClient(RejectReason::GeneralFailure);
                                        return;
                                    }

                                    ReadRequestpacket(std::move(pkt), RequestParseStage::DestPort);
                                });
                    });
        } else {
            size_t bytes = packet->addr_type == kAddrIPv4 ? 4 : 16;
            packet->dest_addr.resize(bytes);
            auto buf = asio::buffer(packet->dest_addr.data(), bytes);
            asio::async_read(
                    client_sock_,
                    buf,
                    [this, self = shared_from_this(), pkt = std::move(packet)](auto ec, auto) mutable {
                        if (ec) {
                            LOG(ERROR) << "Failed to read request dest addr; ec=" << ec;
                            RejectClient(RejectReason::GeneralFailure);
                            return;
                        }

                        ReadRequestpacket(std::move(pkt), RequestParseStage::DestPort);
                    });
        }
        break;
    }
    case RequestParseStage::DestPort: {
        auto buf = asio::buffer(&packet->dest_port, sizeof(packet->dest_port));
        asio::async_read(
                client_sock_,
                buf,
                [this, self = shared_from_this(), pkt = std::move(packet)](auto ec, auto) mutable {
                    if (ec) {
                        LOG(ERROR) << "Failed to read request dest port; ec=" << ec;
                        RejectClient(RejectReason::GeneralFailure);
                        return;
                    }

                    pkt->dest_port = esl::byteswap(pkt->dest_port);
                    ReadRequestpacket(std::move(pkt), RequestParseStage::Complete);
                });
        break;
    }
    case RequestParseStage::Complete:
    default: {
        ConnetRemote(std::move(packet));
    }
    }
}

void Tunnel::ConnetRemote(std::unique_ptr<RequestPacket> packet) {
    std::vector<tcp::endpoint> endpoints;
    if (packet->addr_type == kAddrIPv4) {
        assert(packet->dest_addr.size() == 4);
        auto ip = esl::byteswap(*reinterpret_cast<const uint32_t*>(packet->dest_addr.data()));
        endpoints.emplace_back(asio::ip::make_address_v4(ip), packet->dest_port);
    } else if (packet->addr_type == kAddrIPv6) {
        assert(packet->dest_addr.size() == 16);
        asio::ip::address_v6::bytes_type ip;
        std::transform(packet->dest_addr.begin(), packet->dest_addr.end(), ip.begin(), [](auto b) {
            return static_cast<unsigned char>(b);
        });
        endpoints.emplace_back(asio::ip::make_address_v6(ip), packet->dest_port);
    } else {
        tcp::resolver resolver(executor_);
        std::error_code ec;
        auto results = resolver.resolve(packet->dest_addr, std::to_string(packet->dest_port), ec);
        if (ec) {
            LOG(ERROR) << "Failed to resolve remote address; domain=" << packet->dest_addr
                       << " port=" << packet->dest_port;
            RejectClient(RejectReason::HostUnreachable);
            return;
        }

        std::transform(results.begin(), results.end(), std::back_inserter(endpoints),
                       [](const auto& result) { return result.endpoint(); });
    }

    asio::async_connect(
            dial_sock_,
            endpoints.begin(),
            endpoints.end(),
            [this, self = shared_from_this()](auto ec, std::vector<tcp::endpoint>::iterator it_remote) {
                if (ec) {
                    LOG(ERROR) << "Failed to connect to remote; ec=" << ec;
                    RejectClient(RejectReason::HostUnreachable);
                    return;
                }

                remote_addr_ = *it_remote;

                ReplyRequest();
            });
}

void Tunnel::ReplyRequest() {
    using namespace std::string_literals;
    auto reply = std::make_shared<std::string>("\x05\x00\x00\x01\x00\x00\x00\x00\x10\x10"s);
    auto port_in_be = esl::byteswap(client_sock_.local_endpoint().port());
    memcpy(reply->data() + 8, &port_in_be, sizeof(port_in_be));

    asio::async_write(
            client_sock_,
            asio::buffer(*reply),
            [this, self = shared_from_this(), reply](auto ec, auto bytes) {
                (void)(reply);
                assert(bytes == 10);
                if (ec) {
                    LOG(ERROR) << "Failed to write request reply; ec=" << ec;
                    ShutdownClient();
                    return;
                }

                StartTunneling();
            });
}

void Tunnel::StartTunneling() {
    LOG(INFO) << "Tunneled " << client_addr_ << " <-> " << remote_addr_;

    tcp::no_delay no_delay(true);
    client_sock_.set_option(no_delay);
    dial_sock_.set_option(no_delay);

    constexpr size_t kIOBufSize = 4 * 1024;
    io_client_buf_.resize(kIOBufSize);
    io_remote_buf_.resize(kIOBufSize);

    ForwardRemoteResponse();
    ForwardClientRequest();
}

void Tunnel::ForwardClientRequest() {
    client_sock_.async_read_some(
            asio::buffer(io_client_buf_),
            [this, self = shared_from_this()](std::error_code rec, size_t bytes_read) {
                if (rec) {
                    // Client-side disconnected.
                    if (rec == asio::error::eof) {
                        LOG(INFO) << "Client @ " << client_addr_ << " disconnected";
                        client_sock_disconnected_ = true;
                        ShutdownWriteSafe(dial_sock_, dial_sock_disconnected_);
                        return;
                    }

                    LOG(ERROR) << "Failed to read request from client @ "
                               << client_addr_ << " ec=" << rec;
                    ForceShutdown();
                    return;
                }

                asio::async_write(
                        dial_sock_,
                        asio::buffer(io_client_buf_, bytes_read),
                        [this, self = shared_from_this()](std::error_code wec, size_t /*bytes*/) {
                            if (wec) {
                                LOG(ERROR) << "Failed to write client request to remote @ "
                                           << remote_addr_ << " ec=" << wec;
                                ForceShutdown();
                                return;
                            }

                            ForwardClientRequest();
                        });
            });
}

void Tunnel::ForwardRemoteResponse() {
    dial_sock_.async_read_some(
            asio::buffer(io_remote_buf_),
            [this, self = shared_from_this()](std::error_code rec, size_t bytse_read) {
                if (rec) {
                    // Remote-side disconnected.
                    if (rec == asio::error::eof) {
                        LOG(INFO) << "Remote @ " << remote_addr_ << " disconnected";
                        dial_sock_disconnected_ = true;
                        ShutdownWriteSafe(client_sock_, client_sock_disconnected_);
                        return;
                    }

                    LOG(ERROR) << "Failed to read request from remote @ "
                               << remote_addr_ << " ec=" << rec;
                    ForceShutdown();
                    return;
                }

                asio::async_write(
                        client_sock_,
                        asio::buffer(io_remote_buf_, bytse_read),
                        [this, self = shared_from_this()](std::error_code wec, size_t /*bytes*/) {
                            if (wec) {
                                LOG(ERROR) << "Failed to write remote response to client @ "
                                           << client_addr_ << " ec=" << wec;
                                ForceShutdown();
                                return;
                            }

                            ForwardRemoteResponse();
                        });
            });
}

void Tunnel::ShutdownClient() {
    std::error_code ec;
    client_sock_.shutdown(tcp::socket::shutdown_both, ec);
    DLOG_IF(WARNING, !!ec) << "Failed to shutdown client_sock_; ec=" << ec;
}

void Tunnel::RejectClient(RejectReason reason) {
    std::initializer_list<std::byte> args{
            kSocks5Ver, std::byte(reason), std::byte(0x00), kAddrIPv4, std::byte(0x00), std::byte(0x00),
            std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00)};
    auto reply = std::make_shared<std::vector<std::byte>>(args);

    asio::async_write(
            client_sock_,
            asio::buffer(*reply),
            [this, self = shared_from_this(), reply](auto ec, auto) {
                (void)(reply);
                DLOG_IF(WARNING, !!ec) << "Failed to write reject reply for request; ec=" << ec;
                client_sock_.shutdown(tcp::socket::shutdown_both, ec);
                DLOG_IF(WARNING, !!ec) << "Failed to shutdown client_sock_; ec=" << ec;
            });
}

void Tunnel::ForceShutdown() {
    LOG(INFO) << "Force closing now...";
    std::error_code ec;
    client_sock_.shutdown(tcp::socket::shutdown_send, ec);
    LOG_IF(WARNING, !!ec) << "Failed to shutdown send on client_sock; ec=" << ec;
    dial_sock_.shutdown(tcp::socket::shutdown_send, ec);
    LOG_IF(WARNING, !!ec) << "Failed to shutdown send on dial_sock; ec=" << ec;
}
