//
// Kingsley Chen
//

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include <string_view>

#include "auth_passwords.h"

int main(int argc, const char* argv[]) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

TEST_CASE("Generate password and verify") {
    constexpr std::string_view pwd = "helloworld";
    auto client_token = auth::client::generate_hash_for_password(pwd);
    auto [token, salt] = auth::server::generate_token_for_client_hash(client_token);
    REQUIRE(auth::server::verify_client_hash(client_token, token, salt));
}
