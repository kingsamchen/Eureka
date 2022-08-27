//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "topt/topt.h"

#include <chrono>
#include <cstdint>

#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"

#include "topt/endian.h"
#include "topt/shared_secret_key.h"

namespace topt {
namespace {

std::uint64_t time_counter() {
    auto ts = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch());
    return ts.count() / 30;
}

} // namespace

std::string generate_topt(std::string_view enc_secret) {
    constexpr int digest_size = 20;
    auto secret_key = decode_secret_key(enc_secret);
    CryptoPP::HMAC<CryptoPP::SHA1> hmac_sha1(secret_key.data, secret_key::size);
    std::uint8_t digest[digest_size] = {};
    auto counter = host_to_network(time_counter());
    hmac_sha1.CalculateDigest(digest, reinterpret_cast<CryptoPP::byte*>(&counter), sizeof(counter));
    auto offset = digest[digest_size-1] & 0x0f;
    std::uint32_t value = 0;
    // starting from offset, 4 consecutive bytes.
    std::memcpy(&value, digest + offset, 4);
    value = network_to_host(value);
    return std::to_string(value % 1'000'000);
}

} // namespace topt
