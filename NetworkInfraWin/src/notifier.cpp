/*
 @ 0xCCCCCCCC
*/

#include "notifier.h"

#include "kbase/error_exception_util.h"

#include "event_loop.h"
#include "io_context.h"

namespace network {

Notifier::Notifier(EventLoop* loop, SOCKET socket)
    : loop_(loop), socket_(socket)
{}

void Notifier::HandleCompletionEvent(IOContext* io_context) const
{
    switch (io_context->io_event) {
        case IOEvent::ReadProbe:
            read_complete_event_(io_context);
            break;

        case IOEvent::Read:
            read_complete_event_(io_context);
            break;

        case IOEvent::Write:
            write_complete_event_(io_context);
            break;

        default:
            ENSURE(CHECK, kbase::NotReached()).Require();
    }
}

void Notifier::EnableNotification()
{
    loop_->SubscribeNotifier(this);
}

}   // namespace
