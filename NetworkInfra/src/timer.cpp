/*
 @ 0xCCCCCCCC
*/

#include "timer.h"

#include "kbase/error_exception_util.h"

namespace network {

Timer::Timer(const EventHandler& handler, system_clock::time_point when,
             chrono::microseconds interval)
    : timer_tick_handler_(handler),
      expiration_(when),
      interval_(interval)
{
    ENSURE(CHECK, static_cast<bool>(handler)).Require();
}

void Timer::Tick() const
{
    timer_tick_handler_();
}

void Timer::Restart(system_clock::time_point now)
{
    ENSURE(CHECK, is_repeating()).Require();
    expiration_ = now + interval_;
}

}   // namespace network
