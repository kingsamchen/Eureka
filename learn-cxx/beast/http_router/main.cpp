// Kingsley Chen

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include <esl/strings.h>
#include <fmt/format.h>

#include "http_router/tree.h"

#include "http_router/stringification.h"

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

// Cannot put into anonymous namespace to make sure ADL.
std::ostream& operator<<(std::ostream& os, param p) { // NOLINT(misc-use-internal-linkage)
    os << "(key=" << p.key << ", value=" << p.value << ")";
    return os;
}

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

    int check_priorities() const { // NOLINT
        int prio = 0;
        for (auto child : children()) {
            prio += child.check_priorities();
        }

        if (has_handler()) {
            ++prio;
        }

        INFO("node path=", path());
        CHECK_EQ(prio, n_.priority_);

        return prio;
    }

private:
    const http::node& n_; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

} // namespace http

struct locate_request {
    std::string test_path;
    bool handler_found;
    std::string hit_route;
    std::vector<http::param> params;

    locate_request(std::string_view test,
                   bool found)
        : test_path(test), handler_found(found) {}

    locate_request(std::string_view test,
                   bool found,
                   std::string_view route,
                   std::vector<http::param> ps)
        : test_path(test), handler_found(found), hit_route(route), params(std::move(ps)) {}
};

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

//
// Part 1: build tree
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

TEST_CASE("Priorities of tree") {
    http::node tree;

    SUBCASE("simple routes") {
        constexpr std::string_view paths[]{
            "/hi",
            "/contact",
            "/co",
            "/c",
            "/a",
            "/ab",
            "/doc/",
            "/doc/go_faq.html",
            "/doc/go1.html",
        };

        for (const auto path : paths) {
            tree.add_route(path, http::fake_handler());
        }

        node_test_stub stub(tree);
        stub.check_priorities();
    }

    SUBCASE("wild routes") {
        constexpr std::string_view paths[]{
            "/",
            "/cmd/:tool/:sub",
            "/cmd/:tool/",
            "/src/*filepath",
            "/search/",
            "/search/:query",
            "/user_:name",
            "/user_:name/about",
            "/files/:dir/*filepath",
            "/doc/",
            "/doc/go_faq.html",
            "/doc/go1.html",
            "/info/:user/public",
            "/info/:user/project/:project",
        };

        for (const auto path : paths) {
            tree.add_route(path, http::fake_handler());
        }

        node_test_stub stub(tree);
        stub.check_priorities();
    }
}

//
// Part 2: Locate
//

TEST_CASE("Locate non-wild path") {
    constexpr std::string_view paths[]{
        "/hi",
        "/contact",
        "/co",
        "/c",
        "/a",
        "/ab",
        "/doc/",
        "/doc/go_faq.html",
        "/doc/go1.html",
    };

    http::node tree;
    std::string handler_path;
    for (const auto path : paths) {
        tree.add_route(path, [&handler_path, path](auto, auto) {
            handler_path = path;
        });
    }

    locate_request requests[]{
        {"/a", true},
        {"/", false},
        {"/hi", true},
        {"/contact", true},
        {"/co", true},
        {"/con", false},
        {"/cona", false},
        {"/no", false},
        {"/ab", true},
        {"/doc", false},
        {"/doc/", true},
    };

    std::vector<http::param> params;
    for (const auto& req : requests) {
        auto* handler = tree.locate(req.test_path, params);
        CHECK_EQ(handler != nullptr, req.handler_found);
        if (handler) {
            (*handler)({}, {});
            CHECK_EQ(handler_path, req.test_path);
        }
    }
}

TEST_CASE("Locate wildcard path") {
    constexpr std::string_view paths[]{
        "/",
        "/cmd/:tool/:sub",
        "/cmd/:tool/",
        "/src/*filepath",
        "/search/",
        "/search/:query",
        "/user_:name",
        "/user_:name/about",
        "/files/:dir/*filepath",
        "/doc/",
        "/doc/go_faq.html",
        "/doc/go1.html",
        "/info/:user/public",
        "/info/:user/project/:project",
    };

    http::node tree;
    std::string handler_path;
    for (const auto path : paths) {
        tree.add_route(path, [&handler_path, path](auto, auto) {
            handler_path = path;
        });
    }

    locate_request requests[]{
        {"/", true, "/", {}},
        {"/cmd/test/", true, "/cmd/:tool/", {{.key = "tool", .value = "test"}}},
        {"/cmd/test", false, "", {{.key = "tool", .value = "test"}}},
        {"/cmd/test/3",
         true,
         "/cmd/:tool/:sub",
         {{.key = "tool", .value = "test"}, {.key = "sub", .value = "3"}}},
        {"/src/", true, "/src/*filepath", {{.key = "filepath", .value = "/"}}},
        {"/src/some/file.png",
         true,
         "/src/*filepath",
         {{.key = "filepath", .value = "/some/file.png"}}},
        {"/search/", true, "/search/", {}},
        {"/search/someth!ng+in+ünìcodé",
         true,
         "/search/:query",
         {{.key = "query", .value = "someth!ng+in+ünìcodé"}}},
        {"/search/someth!ng+in+ünìcodé/",
         false,
         "",
         {{.key = "query", .value = "someth!ng+in+ünìcodé"}}},
        {"/user_test", true, "/user_:name", {{.key = "name", .value = "test"}}},
        {"/user_test/about", true, "/user_:name/about", {{.key = "name", .value = "test"}}},
        {"/files/js/inc/framework.js",
         true,
         "/files/:dir/*filepath",
         {{.key = "dir", .value = "js"}, {.key = "filepath", .value = "/inc/framework.js"}}},
        {"/info/gordon/public", true, "/info/:user/public", {{.key = "user", .value = "gordon"}}},
        {"/info/gordon/project/go",
         true,
         "/info/:user/project/:project",
         {{.key = "user", .value = "gordon"}, {.key = "project", .value = "go"}}},
    };

    for (const auto& req : requests) {
        INFO("Test path=", req.test_path);
        std::vector<http::param> params;
        auto* handler = tree.locate(req.test_path, params);
        CHECK_EQ(req.params, params);
        CHECK_EQ(handler != nullptr, req.handler_found);
        if (handler) {
            (*handler)({}, {});
            CHECK_EQ(handler_path, req.hit_route);
        }
    }
}

TEST_CASE("Debug") {
    std::vector<http::param> params;
    params.reserve(8);
    http::node tree;
    tree.add_route("/hello", http::fake_handler());
    tree.add_route("/hello/world", http::fake_handler());
    tree.add_route("/hello/test/*name", http::fake_handler());
    tree.add_route("/user/:name/test", http::fake_handler());
    auto* ptr = tree.locate("/hello/world", params);
    CHECK_NE(ptr, nullptr);
    CHECK(params.empty());
    CHECK_EQ(tree.locate("/hell/no", params), nullptr);
    CHECK(params.empty());
    CHECK_NE(tree.locate("/hello/test/john", params), nullptr);
    CHECK_FALSE(params.empty());
    CHECK_EQ(params[0].key, "name");
    CHECK_EQ(params[0].value, "/john");

    params.clear();
    CHECK_NE(tree.locate("/user/kingsley/test", params), nullptr);
    CHECK_EQ(params[0].key, "name");
    CHECK_EQ(params[0].value, "kingsley");
}
