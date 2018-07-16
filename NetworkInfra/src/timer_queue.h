/*
 @ 0xCCCCCCCC
*/

#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

#include <map>
#include <vector>

#include "kbase/basic_macros.h"

#include "channel.h"
#include "timer.h"

namespace network {

class EventLoop;

class TimerQueue {
public:
    explicit TimerQueue(EventLoop* loop);

    ~TimerQueue();

    DISALLOW_COPY(TimerQueue);

    DISALLOW_MOVE(TimerQueue);

    void AddTimer(const Timer::EventHandler& handler, system_clock::time_point when,
                  chrono::microseconds interval);

    void OnTick();

private:
    bool Insert(Timer&& new_timer);

    void ExtractExpired(chrono::system_clock::time_point sentry_time, std::vector<Timer>& expired);

private:
    EventLoop* loop_;
    int timer_fd_;
    Channel timer_channel_;
    std::multimap<system_clock::time_point, Timer> timers_;
};

}   // namespace network

#endif  // TIMER_QUEUE_H_
