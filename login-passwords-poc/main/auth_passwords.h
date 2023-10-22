//
// Kingsley Chen
//

#pragma once

#include <string>
#include <string_view>

namespace auth {
namespace client {

[[nodiscard]] std::string generate_hash_for_password(std::string_view raw_password);

} // namespace client

namespace server {

[[nodiscard]] std::tuple<std::string, std::string> generate_token_for_client_hash(
        std::string_view client_hash);

[[nodiscard]] bool verify_client_hash(std::string_view client_hash,
                                      std::string_view token,
                                      std::string_view server_salt_hex);

} // namespace server
} // namespace auth
