// Kingsley Chen

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "http_router/tree.h"

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

namespace http {

class node_test_stub {
public:
    using node_type = node::type;

    explicit node_test_stub(const http::node& n)
        : n_(n) {}

    [[nodiscard]] const std::string& path() const {
        return n_.path_;
    }

    [[nodiscard]] const std::string& indices() const {
        return n_.indices_;
    }

    [[nodiscard]] bool has_wild_child() const {
        return n_.has_wild_child_;
    }

    [[nodiscard]] node::type type() const {
        return n_.type_;
    }

    [[nodiscard]] int priroity() const {
        return n_.priority_;
    }

    [[nodiscard]] std::vector<node_test_stub> children() const {
        std::vector<node_test_stub> nodes;
        nodes.reserve(n_.children_.size());
        for (const auto& child : n_.children_) {
            nodes.emplace_back(*child);
        }
        return nodes;
    }

private:
    const http::node& n_; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

} // namespace http

using http::node_test_stub;

TEST_CASE("Find wildcard in path") {
    SUBCASE("no wildcard") {
        constexpr auto result = http::find_wildcard("/hello/name");
        static_assert(!result.found());
        static_assert(!result.valid_name());
    }

    SUBCASE("wildcard is last segment") {
        constexpr auto param = http::find_wildcard("/hello/:name");
        static_assert(param.found());
        static_assert(param.valid_name());
        static_assert(param.pos == 7);
        static_assert(param.name == ":name");

        constexpr auto catch_all = http::find_wildcard("/hello/*name");
        static_assert(catch_all.found());
        static_assert(catch_all.valid_name());
        static_assert(catch_all.pos == 7);
        static_assert(catch_all.name == "*name");
    }

    SUBCASE("wildcard is in the middle") {
        // find the first wildcard.
        constexpr auto param = http::find_wildcard("/hello/:name/:age");
        static_assert(param.found());
        static_assert(param.valid_name());
        static_assert(param.pos == 7);
        static_assert(param.name == ":name");
    }

    SUBCASE("found wildcard but invalid wildcard name") {
        {
            constexpr auto r = http::find_wildcard("/hello/:na:me");
            static_assert(r.found());
            static_assert(!r.valid_name());
        }

        {
            constexpr auto r = http::find_wildcard("/hello/:na*me");
            static_assert(r.found());
            static_assert(!r.valid_name());
        }

        {
            constexpr auto r = http::find_wildcard("/hello/*na:me");
            static_assert(r.found());
            static_assert(!r.valid_name());
        }

        {
            constexpr auto r = http::find_wildcard("/hello/*na*me");
            static_assert(r.found());
            static_assert(!r.valid_name());
        }
    }

    SUBCASE("empty wildcard name is also invalid") {
        {
            constexpr auto r = http::find_wildcard("/hello/:/");
            static_assert(r.found());
            static_assert(!r.valid_name());
        }

        {
            constexpr auto r = http::find_wildcard("/hello/*/");
            static_assert(r.found());
            static_assert(!r.valid_name());
        }
    }
}

TEST_CASE("Longest common prefix") {
    SUBCASE("s1 is sub-string of s2") {
        static_assert(http::longest_common_prefix("abc", "abcdef") == 3);
    }

    SUBCASE("s2 is sub-string of s1") {
        static_assert(http::longest_common_prefix("abcdef", "abc") == 3);
    }

    SUBCASE("not substring but have common prefix") {
        static_assert(http::longest_common_prefix("foobar", "foobaz") == 5);
    }

    SUBCASE("one of str is empty") {
        static_assert(http::longest_common_prefix("", "foobar") == 0);
        static_assert(http::longest_common_prefix("foobar", "") == 0);
    }

    SUBCASE("have none common prefix") {
        static_assert(http::longest_common_prefix("hello", "foobar") == 0);
    }
}

TEST_CASE("Add a route with one parameter") {
    http::node node;
    node.add_route("/hello/:name");

    node_test_stub stub(node);
    CHECK_EQ(stub.path(), "/hello/");
    CHECK_EQ(stub.priroity(), 1);
    CHECK(stub.has_wild_child());
    CHECK_EQ(stub.type(), node_test_stub::node_type::root);

    REQUIRE_EQ(stub.children().size(), 1);
    auto child = stub.children()[0];
    CHECK_EQ(child.path(), ":name");
    CHECK_EQ(child.priroity(), 1);
    CHECK_FALSE(child.has_wild_child());
    CHECK_EQ(child.type(), node_test_stub::node_type::param);
    CHECK(child.children().empty());
}

TEST_CASE("Add a route with one parameter and end with another segment") {
    http::node node;
    node.add_route("/hello/:name/");

    node_test_stub stub(node);
    CHECK_EQ(stub.path(), "/hello/");
    CHECK_EQ(stub.priroity(), 1);
    CHECK(stub.has_wild_child());
    CHECK_EQ(stub.type(), node_test_stub::node_type::root);

    REQUIRE_EQ(stub.children().size(), 1);
    auto child = stub.children()[0];
    CHECK_EQ(child.path(), ":name");
    CHECK_EQ(child.priroity(), 1);
    CHECK_FALSE(child.has_wild_child());
    CHECK_EQ(child.type(), node_test_stub::node_type::param);

    REQUIRE_EQ(child.children().size(), 1);
    auto grand_child = child.children()[0];
    CHECK_EQ(grand_child.path(), "/");
    CHECK_EQ(grand_child.priroity(), 1);
    CHECK_FALSE(grand_child.has_wild_child());
    CHECK_EQ(grand_child.type(), node_test_stub::node_type::plain);
}

TEST_CASE("Add a route with plain path") {
    http::node node;
    node.add_route("/hello/name");

    node_test_stub stub(node);
    CHECK_EQ(stub.path(), "/hello/name");
    CHECK(stub.indices().empty());
    CHECK_FALSE(stub.has_wild_child());
    CHECK_EQ(stub.type(), node_test_stub::node_type::root);
    CHECK_EQ(stub.priroity(), 1);
    CHECK(stub.children().empty());
}
