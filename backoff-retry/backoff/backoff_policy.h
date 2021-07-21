#ifndef BACKOFF_BACKOFF_POLICY_H_
#define BACKOFF_BACKOFF_POLICY_H_

#include <chrono>
#include <cstdint>

namespace backoff {

struct constant_policy {
    template<typename Rep, typename Period>
    auto apply(const std::chrono::duration<Rep, Period>& base, uint32_t /*done_retries*/) const {
        return base;
    }
};

struct linear_policy {
    std::chrono::milliseconds increment;

    explicit linear_policy(std::chrono::milliseconds incr)
        : increment(incr) {}

    template<typename Rep, typename Period>
    auto apply(const std::chrono::duration<Rep, Period>& base, uint32_t done_retries) const {
        return base + done_retries * increment;
    }
};

struct exponential_policy {
    template<typename Rep, typename Period>
    auto apply(const std::chrono::duration<Rep, Period>& base, uint32_t done_retries) const {
        return base * (1 << done_retries);
    }
};

} // namespace backoff

#endif // BACKOFF_BACKOFF_POLICY_H_
