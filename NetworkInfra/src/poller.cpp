/*
 @ 0xCCCCCCCC
*/

#include "poller.h"

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

#include "channel.h"

namespace network {

Poller::Poller(EventLoop* loop)
    : loop_(loop)
{}

chrono::system_clock::time_point Poller::Poll(std::vector<Channel*>& active_channels,
                                              std::chrono::milliseconds timeout)
{
    int active_fd_count = poll(pfds_.data(), pfds_.size(), static_cast<int>(timeout.count()));
    auto now = std::chrono::system_clock::now();
    if (active_fd_count < 0) {
        LOG(WARNING) << "poll() failed: " << errno;
    } else if (active_fd_count == 0) {
        LOG(INFO) << "Nothing happended";
    } else {
        LOG(INFO) << active_fd_count << " fds are active!";
        FillActiveChannels(active_fd_count, active_channels);
    }

    return now;
}

void Poller::UpdateChannel(network::Channel* channel)
{
    auto fd = channel->fd();
    if (channel_table_.count(fd) != 0) {
        auto& pfd = pfds_[channel->poller_info_index()];
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
    } else {
        pollfd new_pfd {};
        new_pfd.fd = fd;
        new_pfd.events = static_cast<short>(channel->events());
        pfds_.push_back(new_pfd);

        channel->set_poller_info_index(static_cast<int>(pfds_.size() - 1));

        channel_table_[fd] = channel;
    }
}

void Poller::FillActiveChannels(int active_count, std::vector<Channel*>& channels)
{
    for (auto it = pfds_.begin(); it != pfds_.end() && active_count > 0; ++it) {
        if (it->revents != Channel::IOEvent::None) {
            auto entry = channel_table_.find(it->fd);
            ENSURE(CHECK, entry != channel_table_.end()).Require();
            Channel* ch = entry->second;
            ENSURE(CHECK, ch->fd() == it->fd)(ch->fd())(it->fd).Require();
            ch->set_received_events(it->revents);
            channels.push_back(ch);
            --active_count;
        }
    }
}

}   // namespace network
