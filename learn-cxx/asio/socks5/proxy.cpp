/*
 @ 0xCCCCCCCC
*/

#include "proxy.h"

#include <cassert>

#include "fmt/format.h"
#include "glog/logging.h"

#include "tunnel.h"

Proxy::Proxy(asio::io_context& ctx, uint16_t port)
    : io_ctx_(ctx),
      listen_addr_(tcp::v4(), port),
      acceptor_(io_ctx_, listen_addr_, true),
      next_worker_id_(0) {}

void Proxy::Start() {
    LOG(INFO) << "socks5 proxy is running at " << acceptor_.local_endpoint();

    uint32_t core_cnt = std::thread::hardware_concurrency();
    for (auto i = 0U; i < core_cnt; ++i) {
        worker_pool_.push_back(EventThread::Create(fmt::format("worker-{0}", i)));
    }

    DoAccept();
}

void Proxy::Stop() {
    acceptor_.cancel();
    asio::post(io_ctx_, [this]() {
        worker_pool_.clear();
        LOG(INFO) << "socks5 proxy stopped";
    });
}

void Proxy::DoAccept() {
    auto executor = PickExecutor();
    acceptor_.async_accept(executor, [this, executor](std::error_code ec, tcp::socket sock) {
        if (ec) {
            if (ec == asio::error::operation_aborted) {
                return;
            }

            LOG(ERROR) << "Failed to accept new connection; ec=" << ec;
        } else {
            assert(executor == sock.get_executor());
            auto tunnel = Tunnel::Make(executor, std::move(sock));
            asio::post(executor, [tunnel = std::move(tunnel)] {
                tunnel->Run();
            });
        }

        DoAccept();
    });
}

asio::io_context::executor_type Proxy::PickExecutor() noexcept {
    auto executor = worker_pool_[next_worker_id_]->GetExecutor();

    ++next_worker_id_;
    if (next_worker_id_ >= worker_pool_.size()) {
        next_worker_id_ = 0;
    }

    return executor;
}
