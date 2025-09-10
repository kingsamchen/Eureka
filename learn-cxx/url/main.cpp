// Kingsley Chen

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <string_view>

#include <boost/url.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace urls = boost::urls;

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

TEST_CASE("Parse url to get query params") {
    constexpr std::string_view url = "http://example.com/search?colors=red&colors=light%2Dblue";
    auto r = urls::parse_uri(url);
    REQUIRE(r.has_value());
    auto qr = r->query();
    fmt::println("query of url={}", qr);
    [[maybe_unused]] auto q = urls::parse_query(r->query());
    REQUIRE(q.has_value());

    for (const auto& qp : r->params()) {
        fmt::println("{} -> {}", qp.key, qp.value);
    }
    fmt::println("");
    for (const auto& qp : r->encoded_params()) {
        fmt::println("{} -> {}", qp.key, qp.value);
    }
    // auto it = r->find("q");
    // REQUIRE(it != r->end());
    // const auto qp(*it);
    // CHECK_EQ(qp.key, "q");
    // CHECK_EQ(qp.value, "hello world");
}

TEST_CASE("Parse query") {
    constexpr std::string_view query = "?colors=red%2Bblue&foo=&bar";
    auto url = urls::parse_origin_form(query);
    REQUIRE(url.has_value());
    REQUIRE_FALSE(url.has_error());
    auto ps = url->params();
    auto it = ps.find("colors");
    REQUIRE(it != ps.end());
    for (const auto& qp : ps) {
        fmt::println("has_value={}\t{} -> {}",qp.has_value, qp.key, qp.value);
    }
}

TEST_CASE("Parse path") {
    constexpr std::string_view query = "/emails/search%2Bfind";
    auto path = urls::parse_path(query);
    INFO(path.error());
    REQUIRE(path.has_value());
    urls::segments_view decoded = *path;
    for (auto seg : decoded) {
        fmt::println("{}", seg);
    }
}
