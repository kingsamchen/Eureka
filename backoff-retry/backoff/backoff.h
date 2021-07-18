#ifndef BACKOFF_BACKOFF_H_
#define BACKOFF_BACKOFF_H_

#include <chrono>
#include <cstdint>
#include <optional>

namespace backoff {

struct constant_policy {
    template<typename Rep, typename Period>
    auto apply(const std::chrono::duration<Rep, Period>& base, uint32_t /*done_retries*/) {
        return base;
    }
};

struct exponential_policy {
    template<typename Rep, typename Period>
    auto apply(const std::chrono::duration<Rep, Period>& base, uint32_t done_retries) {
        return base * (1 << done_retries);
    }
};

template<typename Policy>
class backoff : Policy {
public:
    // TODO: Add static_assert that Policy has a method apply().

    using duration = std::chrono::milliseconds;

    backoff(duration base, uint32_t max_retries)
        : base_(base),
          max_retries_(max_retries) {}

    backoff(const backoff&) = default;

    backoff(backoff&&) = default;

    backoff& operator=(const backoff&) = default;

    backoff& operator=(backoff&&) = default;

    ~backoff() = default;

    std::optional<duration> next_delay() {
        if (done_retries_ == max_retries_) {
            return std::nullopt;
        }

        return std::optional<duration>(this->apply(base_, done_retries_++));
    }

    // TODO: void reset()

private:
    duration base_;
    uint32_t max_retries_;
    uint32_t done_retries_ = 0u;
};

} // namespace backoff

#endif // BACKOFF_BACKOFF_H_
