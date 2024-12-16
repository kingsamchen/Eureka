// Kingsley Chen

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include <esl/strings.h>

#include "http_router/tree.h"

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

namespace http {
namespace {

auto fake_handler() {
    return [](request, response) {
    };
}

} // namespace

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

    [[nodiscard]] bool has_handler() const {
        return !!n_.handler_;
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
            constexpr auto r = http::find_wildcard("/hello:");
            static_assert(r.found());
            static_assert(!r.valid_name());
        }

        {
            constexpr auto r = http::find_wildcard("/hello:/");
            static_assert(r.found());
            static_assert(!r.valid_name());
        }

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

        {
            constexpr auto r = http::find_wildcard("/src/*");
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
    node.add_route("/hello/:name", http::fake_handler());

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
    node.add_route("/hello/:name/", http::fake_handler());

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
    node.add_route("/hello/name", http::fake_handler());

    node_test_stub stub(node);
    CHECK_EQ(stub.path(), "/hello/name");
    CHECK(stub.indices().empty());
    CHECK_FALSE(stub.has_wild_child());
    CHECK_EQ(stub.type(), node_test_stub::node_type::root);
    CHECK_EQ(stub.priroity(), 1);
    CHECK(stub.children().empty());
}

TEST_CASE("Route is a subset of another route") {
    http::node node;
    node.add_route("/:name/foo", http::fake_handler());
    node.add_route("/:name", http::fake_handler());

    node_test_stub stub(node);
    CHECK_EQ(stub.path(), "/");
    CHECK(stub.has_wild_child());
    CHECK_EQ(stub.priroity(), 2);

    auto child = stub.children()[0];
    CHECK_EQ(child.path(), ":name");
    CHECK_FALSE(child.has_wild_child());
    CHECK_EQ(child.priroity(), 2);
    CHECK(child.has_handler());

    CHECK_EQ(child.children()[0].path(), "/foo");
    CHECK_EQ(child.children()[0].priroity(), 1);
    CHECK(child.children()[0].has_handler());
}

//
// Tests adopted
//

TEST_CASE("Only one wildcard per path segment is allowed") {
    http::node tree;

    SUBCASE("case-1") {
        CHECK_THROWS_AS(tree.add_route("/:foo:bar", http::fake_handler()), std::invalid_argument);
    }

    SUBCASE("case-2") {
        CHECK_THROWS_AS(tree.add_route("/:foo:bar/", http::fake_handler()), std::invalid_argument);
    }

    SUBCASE("case-3") {
        CHECK_THROWS_AS(tree.add_route("/:foo*bar", http::fake_handler()), std::invalid_argument);
    }

    SUBCASE("case-4") {
        CHECK_THROWS_AS(tree.add_route("/:foo*bar/", http::fake_handler()), std::invalid_argument);
    }
}

TEST_CASE("Path conflicts with whildcard") {
    http::node tree;

    SUBCASE("case-1") {
        tree.add_route("/cmd/:tool/:sub", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/cmd/vet", http::fake_handler()), std::invalid_argument);
    }

    SUBCASE("case-2") {
        tree.add_route("/search/:query", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/search/invalid", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-3") {
        tree.add_route("/user_:name", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/user_x", http::fake_handler()), std::invalid_argument);
    }

    SUBCASE("case-4") {
        tree.add_route("/id:id", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/id/:id", http::fake_handler()), std::invalid_argument);
    }

    SUBCASE("case-5") {
        tree.add_route("/con:tact", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/conxxx", http::fake_handler()),
                        std::invalid_argument);
        CHECK_THROWS_AS(tree.add_route("/conooo/xxx", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-6") {
        tree.add_route("/src/*filepath", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/src/*filepathx", http::fake_handler()),
                        std::invalid_argument);
        CHECK_THROWS_AS(tree.add_route("/src/", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-7") {
        tree.add_route("/src1/", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/src1/*filepath", http::fake_handler()),
                        std::invalid_argument);
        CHECK_THROWS_AS(tree.add_route("/src2*filepath", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-8") {
        tree.add_route("/who/are/*you", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/who/are/foo", http::fake_handler()),
                        std::invalid_argument);
        CHECK_THROWS_AS(tree.add_route("/who/are/foo/", http::fake_handler()),
                        std::invalid_argument);
        CHECK_THROWS_AS(tree.add_route("/who/are/foo/bar", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("no conflicts") {
        CHECK_NOTHROW(tree.add_route("/cmd/:tool/:sub", http::fake_handler()));
        CHECK_NOTHROW(tree.add_route("/search/:query", http::fake_handler()));
        CHECK_NOTHROW(tree.add_route("/user_:name", http::fake_handler()));
        CHECK_NOTHROW(tree.add_route("/id:id", http::fake_handler()));
        CHECK_NOTHROW(tree.add_route("/src/*filepath", http::fake_handler()));
        CHECK_NOTHROW(tree.add_route("/src1/", http::fake_handler()));
        CHECK_NOTHROW(tree.add_route("/con:tact", http::fake_handler()));
        CHECK_NOTHROW(tree.add_route("/who/are/*you", http::fake_handler()));
        CHECK_NOTHROW(tree.add_route("/who/foo/hello", http::fake_handler()));
    }
}

TEST_CASE("Catch-all conflicts") {
    http::node tree;

    SUBCASE("case-1: conflicts with root") {
        tree.add_route("/", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/*filepath", http::fake_handler()), std::invalid_argument);
    }

    SUBCASE("case-2: catch-all must be the last segment") {
        CHECK_THROWS_AS(tree.add_route("/src/*filepath/x", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-3: catch-all is not the last segment and has prefix with plain path") {
        tree.add_route("/src2/", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/src2/*filepath/x", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-4: catch-all is not the last segment and has prefix with another catch-all") {
        tree.add_route("/src3/*filepath", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/src3/*filepath/x", http::fake_handler()),
                        std::invalid_argument);
    }
}

TEST_CASE("Wildcard conflict error message") {
    auto render_err_msg = [](std::string_view segment,
                             std::string_view full_path,
                             std::string_view wildcard,
                             std::string_view exist_prefix) {
        return fmt::format("'{}' in path '{}' conflicts with existing wildcard '{}' in '{}'",
                           segment, full_path, wildcard, exist_prefix);
    };

    http::node tree;

    SUBCASE("case-1") {
        tree.add_route("/con:tact", http::fake_handler());
        CHECK_THROWS_WITH_AS(tree.add_route("/conxxx", http::fake_handler()),
                             render_err_msg("xxx", "/conxxx", ":tact", "/con:tact").c_str(),
                             std::invalid_argument);
        CHECK_THROWS_WITH_AS(tree.add_route("/conooo/xxx", http::fake_handler()),
                             render_err_msg("ooo", "/conooo/xxx", ":tact", "/con:tact").c_str(),
                             std::invalid_argument);
    }

    SUBCASE("case-2") {
        tree.add_route("/who/are/*you", http::fake_handler());
        CHECK_THROWS_WITH_AS(
                tree.add_route("/who/are/foo", http::fake_handler()),
                render_err_msg("/foo", "/who/are/foo", "/*you", "/who/are/*you").c_str(),
                std::invalid_argument);
        CHECK_THROWS_WITH_AS(
                tree.add_route("/who/are/foo/", http::fake_handler()),
                render_err_msg("/foo/", "/who/are/foo/", "/*you", "/who/are/*you").c_str(),
                std::invalid_argument);
        CHECK_THROWS_WITH_AS(
                tree.add_route("/who/are/foo/bar", http::fake_handler()),
                render_err_msg("/foo/bar", "/who/are/foo/bar", "/*you", "/who/are/*you").c_str(),
                std::invalid_argument);
    }
}

TEST_CASE("Child path conflicts") {
    http::node tree;

    SUBCASE("case-1") {
        tree.add_route("/cmd/vet", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/cmd/:tool/:sub", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-2") {
        tree.add_route("/user_x", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/user_:name", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-3") {
        tree.add_route("/id/:id", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/id:id", http::fake_handler()),
                        std::invalid_argument);
        CHECK_THROWS_AS(tree.add_route("/:id", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-4") {
        tree.add_route("/src/AUTHORS", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/src/*filepath", http::fake_handler()),
                        std::invalid_argument);
    }

    SUBCASE("case-5") {
        tree.add_route("/cmd/vet", http::fake_handler());
        tree.add_route("/src/AUTHORS", http::fake_handler());
        tree.add_route("/user_x", http::fake_handler());
        tree.add_route("/id/:id", http::fake_handler());
        CHECK_THROWS_AS(tree.add_route("/*filepath", http::fake_handler()),
                        std::invalid_argument);
    }
}

TEST_CASE("Path duplicates") {
    constexpr std::string_view paths[]{
        "/",
        "/doc/",
        "/src/*filepath",
        "/search/:query",
        "/user_:name",
    };

    http::node tree;
    for (const auto path : paths) {
        CHECK_NOTHROW(tree.add_route(path, http::fake_handler()));
    }

    for (const auto path : paths) {
        CHECK_THROWS_AS(tree.add_route(path, http::fake_handler()), std::invalid_argument);
    }
}

// TEST_CASE("Debug") {
//     auto r = esl::strings::split("abc", '/').begin();
//     CHECK_EQ(*r, "abc");
// }
