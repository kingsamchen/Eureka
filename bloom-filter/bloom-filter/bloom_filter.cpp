//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "bloom-filter/bloom_filter.h"

#include <cassert>
#include <cmath>
#include <stdexcept>

#include "fmt/format.h"

namespace bf {
namespace {

// Optimal bpe = -(ln(error) / ln(2)^2).
// Throws: `std::invalid_argument`.
double calc_bits_per_entry(double error) {
    constexpr double denom = 0.480453013918201; // ln(2)^2
    auto num = std::log(error);
    auto bpe = -(num / denom);
    assert(bpe > 0);
    if (bpe < 1.) {
        throw std::invalid_argument(fmt::format("invalid bpe: error={} bpe={}", error, bpe));
    }
    return bpe;
}

// Optimal hashes = bpe * ln(2).
std::uint32_t calc_optimal_hashes(double bpe) {
    assert(bpe >= 1.);
    constexpr double base = 0.693147180559945; // ln(2)
    return static_cast<std::uint32_t>(std::ceil(base * bpe));
}

// Returns total bits of the bloom filter, always in power of 2 and 8-bit at minimum.
// Throws: `std::length_error`.
std::uint64_t calc_capacity(std::uint32_t estimated_entries, double bpe) {
    assert(estimated_entries > 0 && bpe >= 1.);
    auto bit_num = static_cast<unsigned int>(std::logb(estimated_entries * bpe));
    // In normal cases, even 2^32-1 entries with low error rate(bpe) will not require bit number
    // more than 52; and any 64-bit integer less than 2^53 can fit in a double.
    if (bit_num > 52) {
        throw std::length_error(fmt::format("required capacity is too large: entries={} bpe={}",
                                            estimated_entries, bpe));
    }

    // Round up. `bits` is always in 2^t
    ++bit_num;
    auto bits = 1llu << bit_num;

    // Require 8-bit at minimum.
    if (bits < 8) {
        bits = 8;
    }

    return bits;
}

} // namespace

bloom_filter::bloom_filter(std::uint32_t estimated_max_entries, double error)
    : error_(error),
      bits_per_entry_(0),
      hashes_(0),
      num_bits_(0),
      max_entries_(0),
      size_(0) {
    if (estimated_max_entries == 0) {
        throw std::invalid_argument("estimated max entries cannot be 0");
    }

    if (error <= 0 || error >= 1.) {
        throw std::invalid_argument(fmt::format("invalid error rate: {}", error));
    }

    bits_per_entry_ = calc_bits_per_entry(error_);
    hashes_ = calc_optimal_hashes(bits_per_entry_);

    num_bits_ = calc_capacity(estimated_max_entries, bits_per_entry_);
    max_entries_ = static_cast<std::size_t>(static_cast<double>(num_bits_) / bits_per_entry_);
    bytes_.resize(num_bits_ / 8);
}

} // namespace bf
