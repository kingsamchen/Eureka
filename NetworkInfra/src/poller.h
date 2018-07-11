/*
 @ 0xCCCCCCCC
*/

#ifndef POLLER_H_
#define POLLER_H_

#include <chrono>
#include <unordered_map>
#include <vector>

#include <poll.h>

#include "kbase/basic_macros.h"

namespace network {

class Channel;
class EventLoop;

namespace chrono = std::chrono;

class Poller {
public:
    explicit Poller(EventLoop* loop);

    ~Poller() = default;

    chrono::system_clock::time_point Poll(std::vector<Channel*>& active_channels,
                                          std::chrono::milliseconds timeout);

    void UpdateChannel(Channel* channel);

private:
    void FillActiveChannels(int active_count, std::vector<Channel*>& channels);

private:
    // Map fd to channel
    using ChannelMap = std::unordered_map<int, Channel*>;

    EventLoop* loop_;
    std::vector<pollfd> pfds_;
    ChannelMap channel_table_;
};

}   // namespace network

#endif // POLLER_H_
