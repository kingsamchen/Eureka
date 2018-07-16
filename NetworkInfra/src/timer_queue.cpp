/*
 @ 0xCCCCCCCC
*/

#include "timer_queue.h"

#include <sys/timerfd.h>
#include <unistd.h>

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

#include "event_loop.h"

namespace {

namespace chrono = std::chrono;

constexpr chrono::microseconds kMinDuration {100};

int CreateTimerFD()
{
    auto fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    ENSURE(CHECK, fd != -1)(errno).Require();
    return fd;
}

timespec ConvertDurationToTimespec(chrono::microseconds duration)
{
    if (duration < kMinDuration) {
        duration = kMinDuration;
    }

    auto sec_part = chrono::duration_cast<chrono::seconds>(duration);
    auto nano_part = chrono::duration_cast<chrono::nanoseconds>(duration - sec_part);

    timespec spec {};
    spec.tv_sec = sec_part.count();
    spec.tv_nsec = nano_part.count();

    return spec;
}

void ResetTimerFD(int timer_fd, chrono::system_clock::time_point expiration)
{
    auto duration = chrono::duration_cast<chrono::microseconds>(
            expiration - chrono::system_clock::now());

    itimerspec new_spec {};
    new_spec.it_value = ConvertDurationToTimespec(duration);
    int rv = timerfd_settime(timer_fd, 0, &new_spec, nullptr);
    ENSURE(CHECK, rv == 0)(errno).Require();
}

void ConsumeTimerFD(int timer_fd)
{
    uint64_t data;
    ssize_t n = read(timer_fd, &data, sizeof(data));
    LOG_IF(INFO, n != sizeof(data)) << "Failed to read from timer-fd!";
}

}   // namespace

namespace network {

TimerQueue::TimerQueue(EventLoop* loop)
    : loop_(loop),
      timer_fd_(CreateTimerFD()),
      timer_channel_(loop, timer_fd_)
{
    timer_channel_.set_read_handler([this] { OnTick(); });
    timer_channel_.EnableReading();
}

TimerQueue::~TimerQueue()
{
    close(timer_fd_);
}

void TimerQueue::AddTimer(const Timer::EventHandler& handler, system_clock::time_point when,
                          chrono::microseconds interval)
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();

    auto new_earliest = Insert(Timer(handler, when, interval));
    if (new_earliest) {
        ResetTimerFD(timer_fd_, when);
    }
}

void TimerQueue::OnTick()
{
    ENSURE(CHECK, loop_->BelongsToCurrentThread()).Require();

    auto sentry = chrono::system_clock::now();

    ConsumeTimerFD(timer_fd_);

    std::vector<Timer> expired;
    ExtractExpired(sentry, expired);

    for (auto& timer : expired) {
        timer.Tick();
        if (timer.is_repeating()) {
            timer.Restart(sentry);
            Insert(std::move(timer));
        }
    }

    if (!timers_.empty()) {
        ResetTimerFD(timer_fd_, timers_.begin()->first);
    }
}

bool TimerQueue::Insert(Timer&& new_timer)
{
    auto when = new_timer.expiration();
    auto it = timers_.cbegin();

    bool earliest_changed = (it == timers_.cend() || when < it->first);

    timers_.emplace(std::make_pair(when, std::move(new_timer)));

    return earliest_changed;
}

void TimerQueue::ExtractExpired(chrono::system_clock::time_point sentry_time,
                                std::vector<Timer>& expired)
{
    auto end = timers_.lower_bound(sentry_time);

    ENSURE(CHECK, end == timers_.end() || sentry_time < end->first).Require();
    for (auto it = timers_.begin(); it != end; ++it) {
        expired.push_back(std::move(it->second));
    }

    timers_.erase(timers_.begin(), end);
}

}   // namespace network
