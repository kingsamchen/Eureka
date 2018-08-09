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

void AssociateWithIOPort(SOCKET socket, HANDLE port, ULONG_PTR completion_key)
{
    bool succeeded = CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), port, completion_key,
                                            0) == port;
    ENSURE(CHECK, succeeded)(kbase::LastError()).Require();
}

}   // namespace

namespace network {

EventPump::EventPump()
    : io_port_(CreateIOPort()),
      io_events_(kInitialEventNum)
{}

EventPump::~EventPump()
{}

void EventPump::Pump(std::chrono::milliseconds timeout,
                     std::vector<std::pair<Notifier*, IOContext*>>& active_notifiers)
{
    // FIXME: Distinguish timeout and failures.
    unsigned long dequeued_num = 0;
    if (!GetQueuedCompletionStatusEx(io_port_.get(),
                                     io_events_.data(),
                                     static_cast<ULONG>(io_events_.size()),
                                     &dequeued_num,
                                     static_cast<DWORD>(timeout.count()),
                                     FALSE)) {
        dequeued_num = 0;
        auto err = WSAGetLastError();
        LOG_IF(WARNING, err != WAIT_TIMEOUT) << "Sever error ocurred: " << err;
    }

    if (dequeued_num == 0) {
        return;
    }

    for (size_t i = 0; i < dequeued_num; ++i) {
        auto& io_event = io_events_[i];

        // notifier could be nullptr as to wakeup the pump.
        auto notifier = reinterpret_cast<Notifier*>(io_event.lpCompletionKey);
        if (!notifier) {
            continue;
        }

        auto ctx = static_cast<IOContext*>(io_event.lpOverlapped);
        ctx->bytes_transferred = io_event.dwNumberOfBytesTransferred;

        active_notifiers.emplace_back(notifier, ctx);
    }
}

void EventPump::Wakeup() const
{
    PostQueuedCompletionStatus(io_port_.get(), 0, 0, nullptr);
}

void EventPump::SubscribeNotifier(Notifier* notifier)
{
    AssociateWithIOPort(notifier->socket(), io_port_.get(), reinterpret_cast<ULONG_PTR>(notifier));
    // TODO: Should we save notifier in a pump maintained notifier table?
}

}   // namespace network
