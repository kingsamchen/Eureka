#ifndef BACKOFF_BACKOFF_POLICY_H_
#define BACKOFF_BACKOFF_POLICY_H_

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <random>

namespace backoff {
namespace details {

inline unsigned seed() {
    return static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count());
}

} // namespace details

using duration_type = std::chrono::milliseconds;

class constant {
public:
    duration_type apply(const duration_type& base, uint32_t /*done_retries*/) const {
        return base;
    }
};

// For policies that support maximum delay: if maximum delay is less than base delay in apply()
// then the maximum delay will always be returned.

class linear {
public:
    explicit linear(const duration_type& incr)
        : increment_(incr),
          max_dur_(duration_type::max()) {}

    linear(const duration_type& incr, const duration_type& max_dur)
        : increment_(incr),
          max_dur_(max_dur) {}

    duration_type apply(const duration_type& base, uint32_t done_retries) const {
        return std::min(base + done_retries * increment_, max_dur_);
    }

private:
    duration_type increment_;
    duration_type max_dur_;
};

class exponential {
public:
    exponential()
        : max_dur_(duration_type::max()) {}

    explicit exponential(const duration_type& max_dur)
        : max_dur_(max_dur) {}

    duration_type apply(const duration_type& base, uint32_t done_retries) const {
        return std::min(base * (1 << done_retries), max_dur_);
    }

private:
    duration_type max_dur_;
};

// rand(0, min(cap, base*2^t))
class exponential_full_jitter {
public:
    exponential_full_jitter()
        : exponential_full_jitter(duration_type::max()) {}

    explicit exponential_full_jitter(const duration_type& max_dur)
        : engine_(details::seed()),
          max_dur_(max_dur) {}

    auto apply(const duration_type& base, uint32_t done_retries) {
        auto dur = std::min(base * (1 << done_retries), max_dur_);
        decltype(rnd_)::param_type range(duration_type{}.count(), dur.count());
        return duration_type(rnd_(engine_, range));
    }

private:
    std::default_random_engine engine_;
    std::uniform_int_distribution<duration_type::rep> rnd_;
    duration_type max_dur_;
};

class exponential_decorrelated_jitter {
public:
    exponential_decorrelated_jitter()
        : exponential_decorrelated_jitter(duration_type::max()) {}

    explicit exponential_decorrelated_jitter(const duration_type& max_dur)
        : engine_(details::seed()),
          max_dur_(max_dur),
          last_dur_{} {}

    auto apply(const duration_type& base, uint32_t /*done_retries*/) {
        if (last_dur_.count() == 0) {
            last_dur_ = base;
        }

        auto bound = last_dur_ * 3;
        decltype(rnd_)::param_type range(base.count(), bound.count());
        duration_type final = std::min(duration_type(rnd_(engine_, range)), max_dur_);

        last_dur_ = final;
        return final;
    }

private:
    std::default_random_engine engine_;
    std::uniform_int_distribution<duration_type::rep> rnd_;
    duration_type max_dur_;
    duration_type last_dur_;
};

} // namespace backoff

#endif // BACKOFF_BACKOFF_POLICY_H_
