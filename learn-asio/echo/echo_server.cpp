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

#include "kbase/at_exit_manager.h"
#include "kbase/basic_macros.h"
#include "kbase/command_line.h"
#include "kbase/logging.h"

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket sock)
        : conn_(std::move(sock)),
          buf_()
    {}

    DISALLOW_COPY(Session);

    void Start()
    {
        DoRead();
    }

private:
    void DoRead()
    {
        conn_.async_read_some(
            asio::buffer(buf_),
            [this, self=shared_from_this()](std::error_code ec, size_t bytes) {
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

    void DoWrite(size_t len)
    {
        asio::async_write(
            conn_,
            asio::buffer(buf_, len),
            [this, self=shared_from_this()](std::error_code ec, size_t) {
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
          signals_(io_ctx_, SIGINT)
    {}

    DISALLOW_COPY(EchoServer);

    DISALLOW_MOVE(EchoServer);

    void Start()
    {
        LOG(INFO) << "EchoServer is running";

        signals_.async_wait([this](auto, auto) {
            LOG(INFO) << "Bye-bye!";
            io_ctx_.stop();
        });

        DoAccept();

        io_ctx_.run();
    }

private:
    void DoAccept()
    {
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

int main(int argc, const char* argv[])
{
    try {
        kbase::AtExitManager exit_manager;

        kbase::LoggingSettings logging_settings;
        logging_settings.logging_destination = kbase::LoggingDestination::LogToSystemDebugLog;
        kbase::ConfigureLoggingSettings(logging_settings);

        kbase::CommandLine::Init(argc, argv);

        auto cmd = kbase::CommandLine::ForCurrentProcess();
        auto port = cmd.GetSwitchValueAs<int>("port", 9876);

        EchoServer srv(static_cast<unsigned short>(port));
        srv.Start();
    } catch (std::exception& ex) {
        LOG(ERROR) << "Unhandled exception: " << ex.what();
    }

    return 0;
}