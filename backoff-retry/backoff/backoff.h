#ifndef BACKOFF_BACKOFF_H_
#define BACKOFF_BACKOFF_H_

#include <chrono>
#include <cstdint>
#include <optional>
#include <type_traits>

#include "backoff/backoff_policy.h"

namespace backoff {

template<typename Policy, typename = void>
struct is_valid_policy : std::false_type {};

template<typename Policy>
struct is_valid_policy<
    Policy,
    std::void_t<
        std::enable_if_t<
            std::is_convertible_v<
                decltype(std::declval<Policy>().apply(duration_type{}, uint32_t{})),
                duration_type>>>>
    : std::true_type {};

template<typename Policy>
class backoff : Policy {
public:
    static_assert(is_valid_policy<Policy>::value,
                  "Policy should have a method apply whose signature meets requirements");

    backoff(duration_type base, uint32_t max_retries)
        : base_(base),
          max_retries_(max_retries) {}

    backoff(duration_type base, uint32_t max_retries, const Policy& policy)
        : Policy(policy),
          base_(base),
          max_retries_(max_retries) {}

    backoff(const backoff&) = default;

    backoff(backoff&&) = default;

    backoff& operator=(const backoff&) = default;

    backoff& operator=(backoff&&) = default;

    ~backoff() = default;

    std::optional<duration_type> next_delay() {
        if (done_retries_ == max_retries_) {
            return std::nullopt;
        }

        return std::optional<duration_type>(this->apply(base_, done_retries_++));
    }

    void reset() noexcept {
        done_retries_ = 0;
    }

private:
    duration_type base_;
    uint32_t max_retries_;
    uint32_t done_retries_ = 0u;
};

inline auto make_constant(const duration_type& delay, uint32_t max_retries) {
    return backoff<constant>(delay, max_retries);
}

inline auto make_linear(const duration_type& base,
                        uint32_t max_retries,
                        const duration_type& increment) {
    return backoff<linear>(base, max_retries, linear(increment));
}

inline auto make_linear(const duration_type& base,
                        uint32_t max_retires,
                        const duration_type& increment,
                        const duration_type& max_delay) {
    return backoff<linear>(base, max_retires, linear(increment, max_delay));
}

inline auto make_exponential(const duration_type& base, uint32_t max_retries) {
    return backoff<exponential>(base, max_retries);
}

inline auto make_exponential(const duration_type& base,
                             uint32_t max_retries,
                             const duration_type& max_delay) {
    return backoff<exponential>(base, max_retries, exponential(max_delay));
}

inline auto make_exponential_full_jitter(const duration_type& base, uint32_t max_retries) {
    return backoff<exponential_full_jitter>(base, max_retries);
}

inline auto make_exponential_full_jitter(const duration_type& base,
                                         uint32_t max_retries,
                                         const duration_type& max_delay) {
    return backoff<exponential_full_jitter>(base, max_retries, exponential_full_jitter(max_delay));
}

inline auto make_exponential_decorrelated_jitter(const duration_type& base, uint32_t max_retries) {
    return backoff<exponential_decorrelated_jitter>(base, max_retries);
}

inline auto make_exponential_decorrelated_jitter(const duration_type& base,
                                                 uint32_t max_retries,
                                                 const duration_type& max_delay) {
    return backoff<exponential_decorrelated_jitter>(base,
                                                    max_retries,
                                                    exponential_decorrelated_jitter(max_delay));
}

} // namespace backoff

#endif // BACKOFF_BACKOFF_H_
