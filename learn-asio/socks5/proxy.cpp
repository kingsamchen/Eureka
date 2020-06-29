/*
 @ 0xCCCCCCCC
*/

#include "proxy.h"

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"
#include "kbase/string_format.h"

#include "tunnel.h"

Proxy::Proxy(asio::io_context& ctx, uint16_t port)
    : io_ctx_(ctx),
      listen_addr_(tcp::v4(), port),
      acceptor_(io_ctx_, listen_addr_, true),
      next_worker_id_(0)
{}

void Proxy::Start()
{
    LOG(INFO) << "socks5 proxy is running at " << acceptor_.local_endpoint();

    uint32_t core_cnt = std::thread::hardware_concurrency();
    for (auto i = 0U; i < core_cnt; ++i) {
        worker_pool_.push_back(EventThread::Create(kbase::StringFormat("worker-{0}", i)));
    }

    DoAccept();
}

void Proxy::Stop()
{
    worker_pool_.clear();
    io_ctx_.stop();
    LOG(INFO) << "socks5 proxy stopped";
}

void Proxy::DoAccept()
{
    auto executor = PickExecutor();
    acceptor_.async_accept(executor, [executor, this](std::error_code ec, tcp::socket sock) {
        if (ec) {
            LOG(ERROR) << "Failed to accept new connection; ec=" << ec;
        } else {
            ENSURE(CHECK, executor == sock.get_executor()).Require();
            auto tunnel = Tunnel::Make(executor, std::move(sock));
            asio::post(executor, [tunnel=std::move(tunnel)] {
                tunnel->Run();
            });
        }

        DoAccept();
    });
}

asio::io_context::executor_type Proxy::PickExecutor() noexcept
{
    auto executor = worker_pool_[next_worker_id_]->GetExecutor();

    ++next_worker_id_;
    if (next_worker_id_ >= worker_pool_.size()) {
        next_worker_id_ = 0;
    }

    return executor;
}
