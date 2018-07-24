/*
 @ 0xCCCCCCCC
*/

#include "channel.h"

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

#include "event_loop.h"

namespace network {

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(IOEvent::None),
      received_events_(IOEvent::None),
      index_(-1),
      handling_events_(false)
{}

Channel::~Channel()
{
    // Invocation of either close-handler or read-handler may trigger disconnection, and during
    // which the poller will remove and destroy the channel which is exactly the one started this.
    // To see how we get round of this, see how a connection is destroyed.
    // `handling_events_` is a canary flag here.
    ENSURE(CHECK, !handling_events_).Require();
}

void Channel::HandleEvents()
{
    handling_events_ = true;

    LOG_IF(WARNING, received_events_ & POLLNVAL) << "Detect POLLNVAL on fd " << fd();

    if ((received_events_ & POLLHUP) && !(received_events_ & POLLIN)) {
        LOG(WARNING) << "Detect POLLHUP on fd " << fd();
        if (close_handler_) {
            close_handler_();
        }
    }

    if (received_events_ & (POLLERR | POLLNVAL)) {
        if (error_handler_) {
            error_handler_();
        }
    }

    if (received_events_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (read_handler_) {
            read_handler_();
        }
    }

    if (received_events_ & POLLOUT) {
        if (write_handler_) {
            write_handler_();
        }
    }

    handling_events_ = false;
}

void Channel::Update()
{
    loop_->UpdateChannel(this);
}

}   // namespace network
