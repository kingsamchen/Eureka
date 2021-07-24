#ifndef BACKOFF_BACKOFF_JITTER_H_
#define BACKOFF_BACKOFF_JITTER_H_

#include <chrono>
#include <random>

#include "backoff/backoff_policy.h"

namespace backoff {
namespace details {

inline unsigned seed() {
    return static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count());
}

} // namespace details

class no_jitter {};

class full_jitter {
public:
    full_jitter()
        : full_jitter(duration_type::min()) {}

    explicit full_jitter(const duration_type& min_delay)
        : engine_(details::seed()),
          minimum_(min_delay) {}

    auto jitter(const duration_type& full_delay) {
        decltype(rnd_)::param_type range(duration_type{}.count(), full_delay.count());
        return std::max(duration_type(rnd_(engine_, range)), minimum_);
    }

private:
    std::default_random_engine engine_;
    std::uniform_int_distribution<duration_type::rep> rnd_;
    duration_type minimum_;
};

class decorrelated_jitter {
public:
    decorrelated_jitter()
        : decorrelated_jitter(duration_type::min()) {}

    explicit decorrelated_jitter(const duration_type& min_delay)
        : engine_(details::seed()),
          minimum_(min_delay),
          last_delay_{} {}

    //auto jitter(const duration_tp)

private:
    std::default_random_engine engine_;
    std::uniform_int_distribution<duration_type::rep> rnd_;
    duration_type minimum_;
    duration_type last_delay_;
};

} // namespace backoff

#endif // BACKOFF_BACKOFF_JITTER_H_
