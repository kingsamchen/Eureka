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
    enum class access_mode {
        read,
        write
    };

public:
    // Throws:
    //  - `std::invalid_argument` if `estimated_max_entries` is not positive or `error` is not
    //    within (0, 1).
    //  - `std::length_error` if calculated capacity is more than 2^50 bytes, which should not
    //    happen in almost any case.
    bloom_filter(std::uint32_t estimated_max_entries, double error);

    bloom_filter(const bloom_filter&) = default;

    bloom_filter(bloom_filter&&) = default;

    bloom_filter& operator=(const bloom_filter&) = default;

    bloom_filter& operator=(bloom_filter&&) = default;

    ~bloom_filter() = default;

    // Returns true if data is added.
    // Returns false if data may already be present.
    // Throws: `std::invalid_argument` if `len` is not positive.
    bool add(const void* data, std::int32_t len);

    // Returns true if data **maybe** contained.
    // Returns false if data is **absolutely not** contained.
    // Throws: `std::invalid_argument` if `len` is not positive.
    bool may_contain(const void* data, std::int32_t len);

    std::size_t max_entries() const noexcept {
        return max_entries_;
    }

    std::uint32_t hash_count() const noexcept {
        return hashes_;
    }

    std::size_t size() const noexcept {
        return size_;
    }

private:
    // In read mode, returns true if hashval is present, returns false otherwise.
    // In write mode, returns true if hashval is added, returns false otherwise.
    // Use 64-bit integer for hash values to prevent data overflow during calculation.
    bool check_add_hash(std::uint64_t hv_a, std::uint64_t hv_b, access_mode mode);

    // Returns true, if the bit at `bit_index` has already been set.
    // Returns false, otherwise.
    bool test_set_bit(std::uint64_t bit_index, access_mode mode);

private:
    double error_;
    double bits_per_entry_;
    std::uint32_t hashes_;    // optimal hash number
    std::uint64_t num_bits_;  // always in power of 2
    std::size_t max_entries_; // optimal max entries with respect to speficied `error_`
    std::size_t size_;        // current count of entries
    std::vector<std::byte> bytes_;
};

} // namespace bf

#endif // BLOOM_FILTER_H_
