//
// Kingsley Chen
//

#include "hashids/hashids.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <stdexcept>
#include <unordered_set>

#include "esl/strings.h"
#include "fmt/format.h"

namespace xx {
namespace {

// The maximum acceptable ratio of len(alphabet)/len(separators)
constexpr double sep_ratio = 3.5;

constexpr double guard_ratio = 12.;

// We assume one integer would produce in general no more than following characters.
constexpr std::size_t encoded_ratio = 7;

constexpr std::string_view default_alphabet =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
constexpr std::string_view base_separators = "cfhistuCFHISTU";

} // namespace

void reorder(const std::string& salt, std::string& str) {
    if (salt.empty()) {
        return;
    }

    std::size_t salt_idx = 0;
    int sum = 0;
    for (auto i = str.size() - 1; i > 0; --i) {
        const int n = static_cast<unsigned char>(salt[salt_idx]);
        sum += n;
        auto j = (salt_idx + static_cast<std::size_t>(n + sum)) % i;
        std::swap(str[i], str[j]);
        salt_idx = (salt_idx + 1) % salt.size();
    }
}

std::string make_alphabet_salt(char lottery, const std::string& salt, const std::string& alphabet) {
    std::string alphabet_salt(1, lottery);
    alphabet_salt.reserve(alphabet.size());
    alphabet_salt.append(salt, 0, std::min(alphabet.size() - 1, salt.size()));
    alphabet_salt.append(alphabet, 0, alphabet.size() - alphabet_salt.size());
    return alphabet_salt;
}

std::string hash_number(std::uint64_t number, const std::string& alphabet) {
    // TODO(KC): reserve space?
    std::string hashed;
    do {
        auto ch = alphabet[number % alphabet.size()];
        hashed += ch;
        number /= alphabet.size();
    } while (number != 0);
    std::reverse(hashed.begin(), hashed.end());
    return hashed;
}

std::uint64_t unhash(std::string_view hashed, std::string_view alphabet) {
    std::uint64_t num{0U};
    for (auto c : hashed) {
        num *= alphabet.size();
        const auto pos = alphabet.find(c);
        num += pos;
    }
    return num;
}

void ensure_minimum_length(std::uint64_t value_hash,
                           std::uint64_t min_len,
                           std::string_view prev_alphabet,
                           std::string_view guards,
                           std::string& encoded) {
    assert(encoded.size() >= 2);

    const auto head_guard_idx =
            (value_hash + static_cast<unsigned char>(encoded[0])) % guards.size();
    encoded.insert(encoded.begin(), guards[head_guard_idx]);

    if (encoded.size() >= min_len) {
        return;
    }

    const auto tail_guard_idx =
            (value_hash + static_cast<unsigned char>(encoded[2])) % guards.size();
    encoded.append(1, guards[tail_guard_idx]);

    std::string alphabet{prev_alphabet};
    for (const auto mid = alphabet.size() / 2; encoded.size() < min_len;) {
        auto salt = alphabet;
        reorder(salt, alphabet);
        encoded.insert(0, alphabet, mid);
        encoded.append(alphabet, 0, mid);
        if (encoded.size() > min_len) {
            const auto excess = encoded.size() - min_len;
            const auto idx = excess / 2;
            encoded.assign(encoded, idx, min_len);
        }
    }
}

hashids::hashids()
    : hashids(options{}) {}

// The original given alphabet will be split into two sub-alphabets:
//  1) separators: by removing characters in the base separators but not in the original alphabet
//     characters in the separators are part of output string, but they are used to separate content
//     parts.
//  2) content alphabet: by removing characters chosen in the separators out of original alphabet
//     characters in content alphabet forms content.
//     however, we still refer content alphabet as the alphabet.
hashids::hashids(const options& opts)
    : min_len_(opts.minimum_length),
      salt_(opts.salt),
      alphabet_(opts.custom_alphabet.empty() ? default_alphabet : opts.custom_alphabet) {
    separators_.reserve(base_separators.size());
    std::copy_if(base_separators.begin(), base_separators.end(), std::back_inserter(separators_),
                 [this](char ch) { return alphabet_.find(ch) != std::string_view::npos; });

    std::unordered_set<char> seen;
    auto end = std::remove_if(alphabet_.begin(), alphabet_.end(), [&seen, this](char ch) {
        const bool shall_remove = seen.count(ch) != 0 || separators_.find(ch) != std::string::npos;
        seen.insert(ch);
        return shall_remove;
    });
    alphabet_.erase(end, alphabet_.end());

    if (separators_.size() + alphabet_.size() < 16) {
        throw std::invalid_argument("Alphabet must contain at least 16 unique characters");
    }

    reorder(salt_, separators_);

    // Ensure that: |content_alphabet|/|separators| <= 3.5
    const auto min_seps_cnt = static_cast<std::int64_t>(
            std::ceil(static_cast<double>(alphabet_.size()) / sep_ratio));
    if (const auto delta = min_seps_cnt - static_cast<std::int64_t>(separators_.size());
        delta > 0) {
        separators_.append(alphabet_, 0, static_cast<std::size_t>(delta));
        alphabet_.erase(0, static_cast<std::size_t>(delta));
    }

    reorder(salt_, alphabet_);

    const auto guards_cnt = static_cast<std::size_t>(
            std::ceil(static_cast<double>(alphabet_.size()) / guard_ratio));
    if (alphabet_.size() < 3) {
        guards_.append(separators_, 0, guards_cnt);
        separators_.erase(0, guards_cnt);
    } else {
        guards_.append(alphabet_, 0, guards_cnt);
        alphabet_.erase(0, guards_cnt);
    }
}

std::string hashids::encode(const std::uint64_t* values, std::size_t len) const {
    assert(values != nullptr);

    if (len == 0) {
        return std::string{};
    }

    std::uint64_t value_hash{0U};
    for (std::size_t i = 0; i < len; ++i) {
        value_hash += values[i] % (i + 100U);
    }

    const char lottery = alphabet_[value_hash % alphabet_.size()];

    // The lottery is always the first character in the result.
    std::string result(1, lottery);
    result.reserve(std::max(len * encoded_ratio, min_len_));

    auto alphabet = alphabet_;
    for (std::size_t i = 0; i < len; ++i) {
        auto alphabet_salt = make_alphabet_salt(lottery, salt_, alphabet);
        reorder(alphabet_salt, alphabet);
        auto number = values[i];
        auto hashed = hash_number(number, alphabet);
        result.append(hashed);
        number %= static_cast<unsigned char>(hashed[0]) + i;
        result.append(1, separators_[number % separators_.size()]);
    }
    result.pop_back();

    if (result.size() < min_len_) {
        ensure_minimum_length(value_hash, min_len_, alphabet, guards_, result);
    }

    return result;
}

std::string hashids::encode_hex(std::string_view hex) const {
    if (hex.empty()) {
        return std::string{};
    }

    constexpr std::size_t group_size = 12U;
    std::vector<std::uint64_t> nums;
    nums.reserve(hex.size() / group_size + 1);
    std::string buf;
    buf.reserve(group_size + 1);
    for (auto part : esl::strings::split(hex, esl::strings::by_length{12})) {
        buf.append(1, '1').append(part);
        std::size_t cvt_len{0U};
        nums.push_back(std::stoull(buf, &cvt_len, 16));
        if (cvt_len != buf.size()) {
            throw std::invalid_argument("invalid hex");
        }
        buf.clear();
    }
    return encode(nums.data(), nums.size());
}

std::vector<std::uint64_t> hashids::decode(std::string_view hashid) const {
    if (hashid.empty()) {
        return {};
    }

    const auto parts = esl::strings::split(hashid, esl::strings::by_any_char{guards_})
                               .to<std::vector<std::string_view>>();
    std::string_view encoded;
    if (parts.size() == 1) {
        encoded = parts[0];
    } else if (parts.size() == 2 || parts.size() == 3) {
        encoded = parts[1];
    } else {
        throw std::invalid_argument("incorrect guarding characters");
    }

    if (encoded.empty()) {
        throw std::invalid_argument("no real data");
    }

    // Remove the lottery.
    const auto lottery = encoded[0];
    encoded.remove_prefix(1);

    std::vector<uint64_t> nums;
    nums.reserve(std::max(encoded.size() / encoded_ratio, static_cast<std::size_t>(1U)));

    auto hashed_view = esl::strings::split(encoded, esl::strings::by_any_char{separators_});
    auto alphabet = alphabet_;
    for (auto hashed : hashed_view) {
        auto alphabet_salt = make_alphabet_salt(lottery, salt_, alphabet);
        reorder(alphabet_salt, alphabet);
        nums.push_back(unhash(hashed, alphabet));
    }

    // Let's do sanity check.
    if (encode(nums.data(), nums.size()) != hashid) {
        throw std::invalid_argument("hashid mismatched");
    }

    return nums;
}

std::string hashids::decode_hex(std::string_view hashid) const {
    if (hashid.empty()) {
        return std::string{};
    }

    std::string hex;
    const auto nums = decode(hashid);
    for (auto n : nums) {
        const auto part = fmt::format("{:x}", n);
        // skip leading '1'
        hex.append(part, 1);
    }
    return hex;
}

} // namespace xx
