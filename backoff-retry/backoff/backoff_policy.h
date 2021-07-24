#ifndef BACKOFF_BACKOFF_POLICY_H_
#define BACKOFF_BACKOFF_POLICY_H_

#include <algorithm>
#include <chrono>
#include <cstdint>

namespace backoff {

using duration_type = std::chrono::milliseconds;

struct constant_policy {
    auto apply(const duration_type& base, uint32_t /*done_retries*/) const {
        return base;
    }
};

// For policies that support maximum delay: if maximum delay is less than base delay in apply()
// then the maximum delay will always be returned.

struct linear_policy {
    duration_type increment;
    duration_type maximum;

    struct options {
        duration_type increment;
        duration_type maximum;

        explicit options(duration_type incr)
            : increment(incr),
              maximum(duration_type::max()) {}

        options(duration_type incr, duration_type max_delay)
            : increment(incr),
              maximum(max_delay) {}
    };

    explicit linear_policy(options opts)
        : increment(opts.increment),
          maximum(opts.maximum) {}

    auto apply(const duration_type& base, uint32_t done_retries) const {
        return std::min(base + done_retries * increment, maximum);
    }
};

struct exponential_policy {
    duration_type maximum;

    struct options {
        duration_type maximum;

        explicit options(duration_type max_delay)
            : maximum(max_delay) {}
    };

    exponential_policy()
        : maximum(duration_type::max()) {}

    explicit exponential_policy(options opts)
        : maximum(opts.maximum) {}

    auto apply(const duration_type& base, uint32_t done_retries) const {
        return std::min(base * (1 << done_retries), maximum);
    }
};

} // namespace backoff

#endif // BACKOFF_BACKOFF_POLICY_H_
