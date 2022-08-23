//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "topt/shared_secret_key.h"

#include <cstring>
#include <random>

#include "cppcodec/base32_rfc4648.hpp"

using base32 = cppcodec::base32_rfc4648;

namespace topt {
namespace {

constexpr char ascii_to_upper(char ch) noexcept {
    return 'a' <= ch && ch <= 'z' ? static_cast<char>('A' + ch - 'a') : ch;
}

} // namespace

secret_key generate_secret_key() {
    static std::mt19937 rand_gen(std::random_device{}());
    uint32_t rands[5];
    std::generate(std::begin(rands), std::end(rands), [] { return rand_gen(); });
    secret_key key; // NOLINT(cppcoreguidelines-pro-type-member-init)
    auto dest = reinterpret_cast<uint32_t*>(key.data);
    static_assert(sizeof(rands) == sizeof(key.data));
    std::memcpy(dest, rands, secret_key::size);
    return key;
}

std::string encode_secret_key(const secret_key& key, bool output_in_group) {
    auto enc = base32::encode(key.data, secret_key::size);
    if (output_in_group) {
        assert(enc.size() == 32);
        enc.reserve(39);
        for (std::size_t i = 1, pos = 4; pos < enc.size(); ++i, pos += 5) {
            enc.insert(pos, 1, ' ');
        }
    }

    return enc;
}

secret_key decode_secret_key(std::string_view encoded) {
    std::string norm_enc;
    norm_enc.reserve(32);
    for (auto ch : encoded) {
        if (ch != ' ') {
            norm_enc.push_back(ascii_to_upper(ch));
        }
    }

    secret_key key; // NOLINT(cppcoreguidelines-pro-type-member-init)
    base32::decode(key.data, secret_key::size, norm_enc);

    return key;
}

} // namespace topt
