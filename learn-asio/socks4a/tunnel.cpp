/*
 @ 0xCCCCCCCC
*/

#include "tunnel.h"

#include "asio/connect.hpp"
#include "asio/read.hpp"
#include "asio/read_until.hpp"
#include "asio/write.hpp"

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

#include "endian_utils.h"

constexpr char kRejectedResp[] = "\x00\x5B\x2B\x2B\x2B\x2B\x2B\x2B";
constexpr auto kRespSize = _countof(kRejectedResp) - 1;

Tunnel::Tunnel(asio::io_context& ctx, tcp::socket&& client_sock)
    : io_ctx_(ctx),
      client_sock_(std::move(client_sock)),
      dial_sock_(ctx)
{
    UNUSED_VAR(io_ctx_);
}

void Tunnel::Run()
{
    ReadRequestPacket(std::make_unique<RequestPacket>(), RequestParseStage::Preamble);
}

void Tunnel::ReadRequestPacket(std::unique_ptr<RequestPacket> packet, RequestParseStage state)
{
    switch (state) {
        case RequestParseStage::Preamble: {
            std::array<asio::mutable_buffer, 4> buffers {
                asio::buffer(&packet->ver, sizeof(packet->ver)),
                asio::buffer(&packet->cmd, sizeof(packet->cmd)),
                asio::buffer(&packet->dest_port, sizeof(packet->dest_port)),
                asio::buffer(&packet->dest_ip, sizeof(packet->dest_ip))
            };

            auto self = shared_from_this();
            asio::async_read(
                client_sock_,
                buffers,
                [self, p=std::move(packet), this](std::error_code ec, size_t bytes) mutable {
                    if (ec) {
                        LOG(ERROR) << "Failed to read request-packet preamble; ec=" << ec;
                        RejectClient();
                        return;
                    }

                    ENSURE(CHECK, bytes == RequestPacket::kPreambleSize)(bytes).Require();
                    p->dest_port = NetworkToHost(p->dest_port);
                    p->dest_ip = NetworkToHost(p->dest_ip);
                    ReadRequestPacket(std::move(p), RequestParseStage::UserId);
                });
            break;
        }
        case RequestParseStage::UserId: {
            auto self = shared_from_this();
            asio::async_read_until(
                client_sock_,
                asio::dynamic_buffer(io_buf_),
                '\0',
                [self, p=std::move(packet), this](std::error_code ec, size_t bytes) mutable {
                    if (ec) {
                        LOG(ERROR) << "Failed to read request-packet userid; ec=" << ec;
                        RejectClient();
                        return;
                    }

                    if (bytes > 1) {
                        p->user_id.assign(io_buf_.data(), bytes - 1);
                    }

                    io_buf_.erase(0, bytes);

                    // For socks4a protocol, the domain field is required.
                    auto next_stage = p->dest_ip < 256 ? RequestParseStage::Domain :
                                                         RequestParseStage::Complete;
                    ReadRequestPacket(std::move(p), next_stage);
                });
            break;
        }
        case RequestParseStage::Domain: {
            auto self = shared_from_this();
            asio::async_read_until(
                client_sock_,
                asio::dynamic_buffer(io_buf_),
                '\0',
                [self, p=std::move(packet), this](std::error_code ec, size_t bytes) mutable {
                    if (ec) {
                        LOG(ERROR) << "Failed to read request-packet the domain field; ec=" << ec;
                        RejectClient();
                        return;
                    }

                    if (bytes > 1) {
                        p->domain.assign(io_buf_.data(), bytes - 1);
                    }

                    io_buf_.clear();

                    ReadRequestPacket(std::move(p), RequestParseStage::Complete);
                });
            break;
        }
        case RequestParseStage::Complete: {
            LOG(INFO) << "Request: " << *packet;
            if (!packet->Valid()) {
                LOG(INFO) << "Invalid request packet; reject the client request";
                RejectClient();
                return;
            }

            ConnectRemote(std::move(packet));
            break;
        }
        default:
            ENSURE(THROW, kbase::NotReached())(state).Require();
            break;
    }
}

void Tunnel::ConnectRemote(std::unique_ptr<RequestPacket> packet)
{
    std::vector<tcp::endpoint> endpoints;

    // If socks4a is enabled, we resolve the domain on the fly.
    // To simplify the code flow, we use synchronous resolve version here.
    if (!packet->domain.empty()) {
        tcp::resolver resolver(io_ctx_);
        std::error_code ec;
        auto results = resolver.resolve(packet->domain, std::to_string(packet->dest_port), ec);
        if (ec) {
            LOG(ERROR) << "Failed to resolve remote address; domain=" << packet->domain
                       << " port=" << packet->dest_port;
            RejectClient();
            return;
        }

        std::transform(results.begin(), results.end(), std::back_inserter(endpoints),
                       [](const auto& result) { return result.endpoint(); });
    } else {
        endpoints.emplace_back(asio::ip::make_address_v4(packet->dest_ip), packet->dest_port);
    }

    asio::async_connect(
        dial_sock_,
        endpoints,
        [this, self=shared_from_this()](std::error_code ec, const tcp::endpoint& endpoint) {
            if (ec) {
                LOG(ERROR) << "Failed to connect remote address; ec=" << ec;
                RejectClient();
                return;
            }

            LOG(INFO) << "Tunneling to" << endpoint;

            // Send back tunneled ack.
            auto port_in_be = HostToNetwork(endpoint.port());
            auto ip_in_be = endpoint.address().to_v4().to_bytes();

            char granted_resp[] = "\x00\x5A\x2A\x2A\x2A\x2A\x2A\x2A";
            memcpy(granted_resp + 2, &port_in_be, sizeof(port_in_be));
            memcpy(granted_resp + 4, &ip_in_be, sizeof(ip_in_be));

            io_buf_.assign(granted_resp, 8);

            asio::async_write(
                client_sock_,
                asio::buffer(io_buf_),
                [this, self=shared_from_this()](std::error_code aec, size_t /*bytes*/) {
                    if (aec) {
                        LOG(ERROR) << "Failed to write tunneled ack back to client; aec" << aec;
                        ForceClose();
                        return;
                    }

                    TransferData();
                });
        });
}

void Tunnel::TransferData()
{
    // TODO:
}

void Tunnel::RejectClient()
{
    asio::async_write(client_sock_, asio::buffer(kRejectedResp, kRespSize),
                      [this, self=shared_from_this()](std::error_code ec, size_t /*bytes*/) {
                          client_sock_.shutdown(tcp::socket::shutdown_send, ec);
                      });
}

void Tunnel::ForceClose()
{
    std::error_code ec;
    client_sock_.shutdown(tcp::socket::shutdown_send, ec);
    LOG_IF(WARNING, !!ec) << "Failed to shutdown send on client_sock; ec=" << ec;
    dial_sock_.shutdown(tcp::socket::shutdown_send, ec);
    LOG_IF(WARNING, !!ec) << "Failed to shutdown send on dial_sock; ec=" << ec;
}
