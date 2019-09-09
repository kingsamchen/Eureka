/*
 @ 0xCCCCCCCC
*/

#include <functional>
#include <iostream>
#include <string>
#include <thread>

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/write.hpp"

#include "kbase/at_exit_manager.h"
#include "kbase/basic_macros.h"
#include "kbase/command_line.h"
#include "kbase/logging.h"

namespace ip = asio::ip;
using ip::tcp;

class EchoClient {
public:
    EchoClient(asio::io_context& io_ctx, const std::string& ip, unsigned short port)
        : io_ctx_(io_ctx),
          remote_addr_(tcp::endpoint(ip::make_address_v4(ip), port)),
          conn_(io_ctx_),
          in_buf_()
    {}

    ~EchoClient()
    {}

    DISALLOW_COPY(EchoClient);

    DISALLOW_MOVE(EchoClient);

    void Start()
    {
        std::cout << "Connecting to " << remote_addr_ << "...\n";
        conn_.async_connect(remote_addr_, [this](std::error_code ec) {
            if (ec) {
                throw std::runtime_error(ec.message());
            }

            std::cout << "Connected!\n";
            DoRead();
        });
    }

    void Stop()
    {
        asio::post(io_ctx_, [this] {
            std::error_code ec;
            conn_.shutdown(tcp::socket::shutdown_send, ec);
            LOG(INFO) << "Shutdown";
        });
    }

    void Send(std::string msg)
    {
        asio::post(io_ctx_, [this, msg=std::move(msg)]() mutable {
            DoWrite(std::move(msg));
        });
    }

private:
    void DoRead()
    {
        conn_.async_read_some(asio::buffer(in_buf_), [this](std::error_code ec, size_t len) {
            if (ec) {
                if (ec == asio::error::eof) {
                    std::cout << "Bye-bye\n";
                    return;
                }

                conn_.close();
                throw std::runtime_error(ec.message());
            }

            std::cout << "<| " << std::string(in_buf_.data(), len) << "\n";
            DoRead();
        });
    }

    void DoWrite(std::string msg)
    {
        out_buf_ = std::move(msg);
        asio::async_write(conn_, asio::buffer(out_buf_), [this](std::error_code ec, size_t) {
            if (ec) {
                conn_.close();
                throw std::runtime_error(ec.message());
            }
        });
    }

private:
    asio::io_context& io_ctx_;
    tcp::endpoint remote_addr_;
    tcp::socket conn_;
    constexpr static size_t kBufSize = 1024;
    std::array<char, kBufSize> in_buf_;
    std::string out_buf_;
};

int main(int argc, const char* argv[])
{
    try {
        kbase::AtExitManager exit_manager;

        kbase::LoggingSettings settings;
        settings.logging_destination = kbase::LogToSystemDebugLog;
        kbase::ConfigureLoggingSettings(settings);

        kbase::CommandLine::Init(argc, argv);

        auto cmdline = kbase::CommandLine::ForCurrentProcess();
        auto ip = cmdline.GetParameter(0);
        auto port = cmdline.GetParameterAs<unsigned short>(1);

        asio::io_context ctx;

        EchoClient cli(ctx, ip, port);
        cli.Start();

        std::thread t([&ctx] {
            try {
                ctx.run();
            } catch (std::exception& ex) {
                LOG(ERROR) << "Error: " << ex.what();
            }

            LOG(INFO) << "IO context exits";
        });

        while (true) {
            std::string s;
            if (!std::getline(std::cin, s)) {
                continue;
            }

            if (s == "/exit") {
                break;
            }

            cli.Send(s);
        }

        cli.Stop();
        t.join();
    } catch (std::exception& ex) {
        LOG(ERROR) << "Unhandled exception: " << ex.what();
    }

    return 0;
}
