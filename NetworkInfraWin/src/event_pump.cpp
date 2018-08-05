/*
 @ 0xCCCCCCCC
*/

#include "event_pump.h"

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

namespace {

kbase::ScopedWinHandle CreateIOPort()
{
    // We use one-loop-per-thread strategy.
    kbase::ScopedWinHandle io_port(CreateIoCompletionPort(INVALID_HANDLE_VALUE,
                                                          nullptr,
                                                          0,
                                                          1));
    ENSURE(CHECK, !!io_port)(kbase::LastError()).Require();
    return io_port;
}

}   // namespace

namespace network {

EventPump::EventPump()
    : io_port_(CreateIOPort()),
      io_events_(kInitialEventNum)
{}

EventPump::~EventPump()
{}

void EventPump::Pump(std::chrono::milliseconds timeout)
{
    // FIXME: Distinguish timeout and failures.
    unsigned long dequeued_num = 0;
    GetQueuedCompletionStatusEx(io_port_.get(),
                                io_events_.data(),
                                io_events_.size(),
                                &dequeued_num,
                                timeout.count(),
                                FALSE);
    if (dequeued_num == 0) {
        LOG(INFO) << "Timeout or some error occurs: " << kbase::LastError();
        return;
    }

    // TODO: Fill active notifiers.

}

}   // namespace network
