#ifndef BACKOFF_BACKOFF_H_
#define BACKOFF_BACKOFF_H_

#include <chrono>
#include <cstdint>
#include <optional>
#include <type_traits>

#include "backoff/backoff_jitter.h"
#include "backoff/backoff_policy.h"

#if defined(_WIN32) || defined(_WIN64)
#define EMPTY_BASES __declspec(empty_bases)
#else
#define EMPTY_BASES
#endif

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

template<typename Policy, typename Jitter = no_jitter>
class EMPTY_BASES backoff : Policy, Jitter {
public:
    static_assert(is_valid_policy<Policy>::value,
                  "Policy should have a method apply whose signature meets requirements");

    // TODO: static_assert on Jitter traits

    backoff(duration_type base, uint32_t max_retries)
        : base_(base),
          max_retries_(max_retries) {}

    backoff(duration_type base, uint32_t max_retries, const Jitter& jitter)
        : Jitter(jitter),
          base_(base),
          max_retries_(max_retries) {}

    backoff(duration_type base, uint32_t max_retries, const Policy& policy)
        : Policy(policy),
          base_(base),
          max_retries_(max_retries) {}

    backoff(duration_type base, uint32_t max_retries, const Policy& policy, const Jitter& jitter)
        : Policy(policy),
          Jitter(jitter),
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

        auto delay = this->apply(base_, done_retries_++);
        if constexpr (!std::is_same_v<Jitter, no_jitter>) {
            delay = this->jitter(delay);
        }

        return std::optional<duration_type>(delay);
    }

    void reset() noexcept {
        done_retries_ = 0;
    }

private:
    duration_type base_;
    uint32_t max_retries_;
    uint32_t done_retries_ = 0u;
};

template<typename Jitter>
auto make_constant(duration_type delay, uint32_t max_retries, Jitter&& jitter)
    -> backoff<constant_policy, Jitter> {
    return backoff<constant_policy, Jitter>(delay, max_retries, std::forward<Jitter>(jitter));
}

inline auto make_constant(duration_type delay, uint32_t max_retries) -> backoff<constant_policy> {
    return backoff<constant_policy>(delay, max_retries);
}

template<typename Jitter>
auto make_linear(duration_type base, uint32_t max_retries, linear_policy::options opts,
                 Jitter&& jitter) -> backoff<linear_policy, Jitter> {
    return backoff<linear_policy, Jitter>(base,
                                          max_retries,
                                          linear_policy(opts),
                                          std::forward<Jitter>(jitter));
}

inline auto make_linear(duration_type base, uint32_t max_retries, duration_type increment)
    -> backoff<linear_policy> {
    linear_policy::options opts(increment);
    return backoff<linear_policy>(base, max_retries, linear_policy(opts));
}

} // namespace backoff

#endif // BACKOFF_BACKOFF_H_
