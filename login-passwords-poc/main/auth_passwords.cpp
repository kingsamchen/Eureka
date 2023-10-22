//
// Kingsley Chen
//

#include "auth_passwords.h"

#include <tuple>
#include <vector>

#include "botan/bcrypt.h"
#include "botan/hash.h"
#include "botan/hex.h"
#include "botan/system_rng.h"

namespace auth {
namespace client {

constexpr std::uint16_t hash_work_factor = 12;
constexpr char hash_salt[] = "this is a test text";

std::string generate_hash_for_password(std::string_view raw_password) {
    const std::string salted_pwd = std::string{raw_password} + hash_salt;
    auto client_hash = Botan::generate_bcrypt(salted_pwd, Botan::system_rng(), hash_work_factor);
    return client_hash;
}

} // namespace client

namespace server {

std::tuple<std::string, std::string> generate_token_for_client_hash(std::string_view client_hash) {
    std::vector<std::uint8_t> server_salt(client_hash.size());
    Botan::system_rng().randomize(server_salt);

    // If high-security is required, can use hash functions like pbkdf or bcrypt instead.
    // However, using one of these cost-aware functions may have perceivable negative performance
    // impact on server side.
    auto hash = Botan::HashFunction::create_or_throw("SHA-256");
    hash->update(client_hash);
    hash->update(server_salt);
    auto token = Botan::hex_encode(hash->final());

    return {std::move(token), Botan::hex_encode(server_salt)};
}

bool verify_client_hash(std::string_view client_hash,
                        std::string_view token,
                        std::string_view server_salt_hex) {
    const auto server_salt = Botan::hex_decode(server_salt_hex);
    auto hash = Botan::HashFunction::create_or_throw("SHA-256");
    hash->update(client_hash);
    hash->update(server_salt);
    return Botan::hex_encode(hash->final()) == token;
}

} // namespace server
} // namespace auth