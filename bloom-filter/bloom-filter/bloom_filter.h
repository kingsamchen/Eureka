//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef BLOOM_FILTER_H_
#define BLOOM_FILTER_H_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace bf {

class bloom_filter {
public:
    // Throws:
    // - `std::invalid_argument` if `estimated_max_entries` is not positive or `error` is not
    //   within (0, 1).
    // - `std::length_error` if calculated capacity is more than 2^50 bytes, which should not
    //   happen in almost any case.
    bloom_filter(std::uint32_t estimated_max_entries, double error);

    bloom_filter(const bloom_filter&) = default;

    bloom_filter(bloom_filter&&) = default;

    bloom_filter& operator=(const bloom_filter&) = default;

    bloom_filter& operator=(bloom_filter&&) = default;

    ~bloom_filter() = default;

    std::size_t max_entries() const noexcept {
        return max_entries_;
    }

private:
    double error_;
    double bits_per_entry_;
    std::uint32_t hashes_;
    std::uint64_t num_bits_;
    std::size_t max_entries_;
    std::size_t size_;
    std::vector<std::byte> bytes_;
};

} // namespace bf

#endif // BLOOM_FILTER_H_
