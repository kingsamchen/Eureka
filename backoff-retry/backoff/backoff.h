#ifndef BACKOFF_BACKOFF_H_
#define BACKOFF_BACKOFF_H_

#include <chrono>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace backoff {

using duration_type = std::chrono::milliseconds;

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

} // namespace backoff

#endif // BACKOFF_BACKOFF_H_
