#ifndef BACKOFF_BACKOFF_ATTEMPT_H_
#define BACKOFF_BACKOFF_ATTEMPT_H_

#include <thread>

#include "backoff/backoff.h"

namespace backoff {

struct attempt_result {
    enum result_code {
        success,
        attempt_failure,
        hard_error // non-retryable
    };

    result_code code;

    // implicit
    attempt_result(result_code code)
        : code(code) {}

    bool ok() const noexcept {
        return code == result_code::success;
    }
};

template<typename Policy, typename F>
attempt_result attempt(backoff<Policy>&& backoff, F&& fn) {
    while (true) {
        // either result_code::success or result_code::hard_error
        if (attempt_result result = fn(); result.code != attempt_result::attempt_failure) {
            return result;
        }

        auto delay = backoff.next_delay();
        // Maxmium retries reached.
        if (!delay) {
            return attempt_result::attempt_failure;
        }

        std::this_thread::sleep_for(*delay);
    }
}

} // namespace backoff

#endif // BACKOFF_BACKOFF_ATTEMPT_H_
