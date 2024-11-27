/*
 @ 0xCCCCCCCC
*/

#include <array>
#include <memory>
#include <system_error>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/signal_set.hpp"
#include "asio/write.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

using asio::ip::tcp;

DEFINE_uint32(port, 9876, "listening port");

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket sock)
        : conn_(std::move(sock)),
          buf_() {}

    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    void Start() {
        DoRead();
    }

private:
    void DoRead() {
        conn_.async_read_some(
                asio::buffer(buf_),
                [this, self = shared_from_this()](std::error_code ec, size_t bytes) {
                    if (ec) {
                        if (ec == asio::error::eof) {
                            LOG(INFO) << "Remote connection @ " << conn_.remote_endpoint()
                                      << "disconnected";
                        } else {
                            LOG(ERROR) << "Failed to read from connection; "
                                       << "remote=" << conn_.remote_endpoint() << " ec=" << ec;
                        }

                        return;
                    }

                    DoWrite(bytes);
                });
    }

    void DoWrite(size_t len) {
        asio::async_write(
                conn_,
                asio::buffer(buf_, len),
                [this, self = shared_from_this()](std::error_code ec, size_t) {
                    if (ec) {
                        LOG(ERROR) << "Failed to write to connection; remote="
                                   << conn_.remote_endpoint() << " ec=" << ec;
                        return;
                    }
                    DoRead();
                });
    }

private:
    tcp::socket conn_;
    constexpr static size_t kBufSize = 1024;
    std::array<char, kBufSize> buf_;
};

class EchoServer {
public:
    explicit EchoServer(unsigned short port)
        : acceptor_(io_ctx_, tcp::endpoint(tcp::v4(), port)),
          signals_(io_ctx_, SIGINT) {}

    EchoServer(const EchoServer&) = delete;
    EchoServer(EchoServer&&) = delete;
    EchoServer& operator=(const EchoServer&) = delete;
    EchoServer& operator=(EchoServer&&) = delete;

    void Start() {
        LOG(INFO) << "EchoServer is running";

        signals_.async_wait([this](auto, auto) {
            LOG(INFO) << "Bye-bye!";
            io_ctx_.stop();
        });

        DoAccept();

        io_ctx_.run();
    }

private:
    void DoAccept() {
        acceptor_.async_accept([this](std::error_code ec, tcp::socket peer) {
            if (ec) {
                LOG(ERROR) << "Failed to accept new connection; " << ec;
            } else {
                LOG(INFO) << "New client connected @ " << peer.remote_endpoint();
                auto conn = std::make_shared<Session>(std::move(peer));
                conn->Start();
            }

            DoAccept();
        });
    }

private:
    asio::io_context io_ctx_;
    tcp::acceptor acceptor_;
    asio::signal_set signals_;
};

void RunServer() {
    EchoServer srv(static_cast<unsigned short>(FLAGS_port));
    srv.Start();
}

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    try {
        RunServer();
    } catch (std::exception& ex) {
        LOG(ERROR) << "Unhandled exception: " << ex.what();
    }

    return 0;
}