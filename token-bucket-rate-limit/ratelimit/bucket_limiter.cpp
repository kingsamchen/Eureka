/*
 @ 0xCCCCCCCC
*/

#include "bucket_limiter.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <thread>

Bucket::Bucket(int64_t cap, Clock::duration fill_tick, int64_t quantum)
    : capacity_(cap),
      available_(cap),
      fill_tick_(fill_tick),
      quantum_(quantum),
      start_time_(Clock::now()),
      latest_tick_(0)
{
    if (cap <= 0) {
        throw std::invalid_argument("cap must be positive");
    }

    if (quantum <= 0 || quantum > cap) {
        throw std::invalid_argument("quantum must be positive and must not be greater than cap");
    }
}

void Bucket::Wait(int64_t count)
{
    auto dur = Take(count);
    if (dur.count() > 0) {
        std::this_thread::sleep_for(dur);
    }
}

auto Bucket::Take(int64_t count) -> Clock::duration
{
    assert(count >= 0);

    if (count <= 0) {
        return Clock::duration(0);
    }

    auto ticks_since_start = (Clock::now() - start_time_) / fill_tick_;
    Clock::duration dur;

    {
        std::lock_guard<std::mutex> lock(mtx_);
        dur = DoTake(ticks_since_start, count);
    }

    return dur;
}

int64_t Bucket::TakeAvailable(int64_t count)
{
    assert(count >= 0);

    if (count <= 0) {
        return 0;
    }

    auto ticks_since_start = (Clock::now() - start_time_) / fill_tick_;
    int64_t taken = 0;

    {
        std::lock_guard<std::mutex> lock(mtx_);
        taken = DoTakeAvailable(ticks_since_start, count);
    }

    return taken;
}

int64_t Bucket::DoTakeAvailable(int64_t current_ticks, int64_t count)
{
    AdjustAvailableTokens(current_ticks);

    // Still might be negaive.
    if (available_ <= 0) {
        return 0;
    }

    auto taken = std::min(count, available_);
    available_ -= taken;

    return taken;
}

auto Bucket::DoTake(int64_t current_ticks, int64_t count) -> Clock::duration
{
    AdjustAvailableTokens(current_ticks);

    auto remain = available_ - count;
    if (remain >= 0) {
        available_ = remain;
        return Clock::duration(0);
    }

    auto wait_ticks = (-remain + quantum_ - 1) / quantum_;
    available_ = remain;
    return wait_ticks * fill_tick_;
}

void Bucket::AdjustAvailableTokens(int64_t current_tick)
{
    assert(current_tick >= latest_tick_);

    auto last_tick = latest_tick_;
    latest_tick_ = current_tick;

    if (available_ >= capacity_) {
        return;
    }

    auto filled_tokens = (current_tick - last_tick) * quantum_;
    available_ = std::min(available_ + filled_tokens, capacity_);
}
