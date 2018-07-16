/*
 @ 0xCCCCCCCC
*/

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <functional>

#include "kbase/basic_macros.h"

namespace network {

namespace chrono = std::chrono;

using chrono::system_clock;

class Timer {
public:
    using EventHandler = std::function<void()>;

    Timer(const EventHandler& handler, system_clock::time_point when,
          chrono::microseconds interval);

    ~Timer() = default;

    DISALLOW_COPY(Timer);

    DEFAULT_MOVE(Timer);

    void Tick() const;

    // Restart the repeating timer from now.
    void Restart(system_clock::time_point now);

    system_clock::time_point expiration() const noexcept
    {
        return expiration_;
    }

    bool is_repeating() const noexcept
    {
        return interval_.count() > 0;
    }

private:
    EventHandler timer_tick_handler_;
    system_clock::time_point expiration_;
    chrono::microseconds interval_;
};

}   // network

#endif // TIMER_H_
