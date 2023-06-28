//
// Kingsley Chen
//

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace xx {

class hashids {
public:
    struct options {
        std::string_view salt;
        std::string_view custom_alphabet;
        std::size_t minimum_length{0};
    };

    hashids();

    // Throws
    //  - `std::invalid_argument` if custom_alphabet doesn't meet requirements.
    explicit hashids(const options& opts);

    // Default copy & move

    // If `len` is 0 the encoded result is an empty string.
    // `values` should use span if available.
    std::string encode(const std::uint64_t* values, std::size_t len) const;

    // hex should not prefix with `0x`
    // Throws:
    //  - `std::invalid_argument` if `hex` is not valid.
    std::string encode_hex(std::string_view hex) const;

    // If `hashid` is empty, returns empty number sequence.
    // Throws
    //  - `std::invalid_argument` if hashid is invalid or wasn't generated by this instance.
    std::vector<std::uint64_t> decode(std::string_view hashid) const;

    // If `hashid` is empty, returns empty number sequence.
    // Throws
    //  - `std::invalid_argument` if hashid is invalid or wasn't generated by this instance.
    std::string decode_hex(std::string_view hashid) const;

private:
    std::size_t min_len_;
    std::string salt_;
    std::string alphabet_;
    std::string separators_;
    std::string guards_;
};

} // namespace xx
