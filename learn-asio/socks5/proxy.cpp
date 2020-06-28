/*
 @ 0xCCCCCCCC
*/

#include "proxy.h"

#include "kbase/logging.h"
#include "kbase/string_format.h"

Proxy::Proxy(asio::io_context& ctx, uint16_t port)
    : io_ctx_(ctx),
      listen_addr_(tcp::v4(), port),
      acceptor_(io_ctx_, listen_addr_, true),
      next_worker_id_(0)
{}

void Proxy::Start()
{
    uint32_t core_cnt = std::thread::hardware_concurrency();
    for (auto i = 0U; i < core_cnt; ++i) {
        worker_pool_.push_back(EventThread::Create(kbase::StringFormat("worker-{0}", i)));
    }

    // TODO: DoAccept()
}

void Proxy::Stop()
{
    worker_pool_.clear();
    io_ctx_.stop();
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
