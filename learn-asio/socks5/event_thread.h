/*
 @ 0xCCCCCCCC
*/

#ifndef LEARN_ASIO_SOCKS5_EVENT_THREAD_H_
#define LEARN_ASIO_SOCKS5_EVENT_THREAD_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "kbase/basic_macros.h"

#include "asio/io_context.hpp"

class EventThread {
public:
    ~EventThread();

    DISALLOW_COPY(EventThread);

    DISALLOW_MOVE(EventThread);

    static std::unique_ptr<EventThread> Create(std::string name);

    asio::io_context::executor_type GetExecutor() noexcept;

private:
    explicit EventThread(std::string name);

    void ThreadMain();

private:
    std::string name_;
    asio::io_context io_ctx_;
    bool started_;
    std::mutex start_mtx_;
    std::condition_variable start_cv_;
    std::thread raw_thread_;
};

#endif  // LEARN_ASIO_SOCKS5_EVENT_THREAD_H_
