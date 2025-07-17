/*
 @ 0xCCCCCCCC
*/

#include <cstdint>
#include <sstream>

#include <asio/ip/address.hpp>
#include <asio/ip/tcp.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

namespace {

asio::ip::tcp::endpoint make_endpoint(const std::string& addr, std::uint16_t port) {
    return asio::ip::tcp::endpoint(asio::ip::make_address(addr), port);
}

asio::ip::tcp::endpoint make_endpoint(std::uint16_t port) {
    return make_endpoint("0.0.0.0", port);
}

} // namespace

TEST_CASE("Make tcp endpoint") {
    auto ep = make_endpoint(8080);
    CHECK_EQ(ep.port(), 8080);
    CHECK(ep.address().is_v4());
    CHECK_EQ(ep.address().to_string(), "0.0.0.0");
}

TEST_CASE("Make ipv6 tcp endpoint") {
    auto ep = make_endpoint("::", 9876);
    CHECK_EQ(ep.port(), 9876);
    CHECK(ep.address().is_v6());
    CHECK_EQ(ep.address().to_string(), "::");
}

TEST_CASE("Default endpoint") {
    asio::ip::tcp::endpoint ep;
    std::ostringstream oss;
    oss << ep;
    CHECK_EQ(oss.str(), "0.0.0.0:0");
}
