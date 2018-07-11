/*
 @ 0xCCCCCCCC
*/

#include "channel.h"

#include "event_loop.h"

namespace network {

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(IOEvent::None),
      received_events_(IOEvent::None),
      index_(-1)
{}

void Channel::HandleEvents()
{
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
}

void Channel::Update()
{
    loop_->UpdateChannel(this);
}

}   // namespace network
