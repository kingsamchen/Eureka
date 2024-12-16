// Kingsley Chen

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "http_router/tree.h"

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

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
        static_assert(param.wildcard_name == ":name");

        constexpr auto catch_all = http::find_wildcard("/hello/*name");
        static_assert(catch_all.found());
        static_assert(catch_all.valid_name());
        static_assert(catch_all.pos == 7);
        static_assert(catch_all.wildcard_name == "*name");
    }

    SUBCASE("wildcard is in the middle") {
        // find the first wildcard.
        constexpr auto param = http::find_wildcard("/hello/:name/:age");
        static_assert(param.found());
        static_assert(param.valid_name());
        static_assert(param.pos == 7);
        static_assert(param.wildcard_name == ":name");
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
}
