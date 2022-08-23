//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef TOPT_SHARED_SECRET_KEY_H_
#define TOPT_SHARED_SECRET_KEY_H_

#include <cstdint>
#include <string_view>

namespace topt {

struct secret_key {
    inline static constexpr std::size_t size = 20;
    std::uint8_t data[size];
};

secret_key generate_secret_key();

std::string encode_secret_key(const secret_key& key, bool output_in_group);

// Throws for invalid encoded secret.
secret_key decode_secret_key(std::string_view encoded);

} // namespace topt

#endif // TOPT_SHARED_SECRET_KEY_H_
