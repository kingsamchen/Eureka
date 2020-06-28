/*
 @ 0xCCCCCCCC
*/

#include "event_thread.h"

#include "asio/executor_work_guard.hpp"

#include "kbase/logging.h"

EventThread::EventThread(std::string name)
    : name_(std::move(name)),
      started_(false)
{
    raw_thread_ = std::thread(&EventThread::ThreadMain, this);

    {
        std::unique_lock lock(start_mtx_);
        start_cv_.wait(lock, [this] { return started_; });
    }
}

EventThread::~EventThread()
{
    try {
        io_ctx_.stop();
        raw_thread_.join();
    } catch (const std::exception& ex) {
        DLOG(WARNING) << "Unexpected exception: " << ex.what() ;
    }

    DLOG(INFO) << name_ << " exited";
}

// static
std::unique_ptr<EventThread> EventThread::Create(std::string name)
{
    std::unique_ptr<EventThread> thread(new EventThread(std::move(name)));
    return thread;
}

void EventThread::ThreadMain()
{
    {
        std::lock_guard lock(start_mtx_);
        started_ = true;
    }

    start_cv_.notify_one();

    DLOG(INFO) << name_ << " is running";

    [[maybe_unused]] auto work_guard = asio::make_work_guard(io_ctx_);
    io_ctx_.run();
}

asio::io_context::executor_type EventThread::GetExecutor() noexcept
{
    return io_ctx_.get_executor();
}
