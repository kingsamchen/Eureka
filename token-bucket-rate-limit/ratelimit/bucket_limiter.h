/*
 @ 0xCCCCCCCC
*/

#ifndef RATE_LIMIT_BUCKET_LIMITER_H_
#define RATE_LIMIT_BUCKET_LIMITER_H_

#include <chrono>
#include <cstdint>
#include <mutex>

class Bucket {
public:
    using Clock = std::chrono::system_clock;

    Bucket(int64_t cap, Clock::duration fill_tick, int64_t quantum);

    ~Bucket() = default;

    Bucket(const Bucket&) = delete;

    Bucket(Bucket&&) = delete;

    Bucket& operator=(const Bucket&) = delete;

    Bucket& operator=(Bucket&&) = delete;

    // Likes Take(), but waits immedidately when running out of the bucket.
    void Wait(int64_t count);

    // Takes `count` tokens from the bucket.
    // If available tokens are enough to service the request, it returns 0-duraiton.
    // Otherwise, the available number becomes negative, and the function returns the
    // duration to indicate number of ticks should be paied for the next take.
    Clock::duration Take(int64_t count);

    // Takes tokens, with `count` at maximum, from the bucket.
    // It does not block and instead returns 0 when available tokens are short.
    int64_t TakeAvailable(int64_t count);

private:
    Clock::duration DoTake(int64_t current_ticks, int64_t count);

    int64_t DoTakeAvailable(int64_t current_ticks, int64_t count);

    void AdjustAvailableTokens(int64_t current_tick);

private:
    const int64_t capacity_;
    int64_t available_; // could be negative.
    const Clock::duration fill_tick_;
    const int64_t quantum_;   // number of tokens added on each tick.
    Clock::time_point start_time_;
    int64_t latest_tick_;
    std::mutex mtx_;
};

#endif  // RATE_LIMIT_BUCKET_LIMITER_H_
