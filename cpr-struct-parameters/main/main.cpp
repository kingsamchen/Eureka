//
// Kingsley Chen
//

#include <iostream>
#include <string>
#include <type_traits>

#include "cpr/cpr.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

inline const std::string& stringify_member(const std::string& m) {
    return m;
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
std::string stringify_member(T value) {
    return std::to_string(value);
}

#define CPR_ADD_PARAMETER(member, name) \
    { p.Add({name, stringify_member(m.member)}); }

#define CPR_PARAMETERS_EXPAND_OP_2ARGS(fn, x, y, next) fn(x, y) CPR_PARAMETERS_EXPAND_##fn##_##next

#define CPR_PARAMETERS_OP_2ARGS_A(fn, x, y) CPR_PARAMETERS_EXPAND_OP_2ARGS(fn, x, y, B)
#define CPR_PARAMETERS_OP_2ARGS_B(fn, x, y) CPR_PARAMETERS_EXPAND_OP_2ARGS(fn, x, y, A)

#define CPR_PARAMETERS_OP_1ARGS(fn, x)
#define CPR_PARAMETERS_OP_0ARGS(fn)

#define CPR_PARAMETERS_CHOOSE_OP(_0, _1, _2, name, ...) name

#define CPR_PARAMETERS_EXPAND_CPR_ADD_PARAMETER_A(...)  \
    CPR_PARAMETERS_CHOOSE_OP(CPR_PARAMETERS_OP_0ARGS,   \
                             ##__VA_ARGS__,             \
                             CPR_PARAMETERS_OP_2ARGS_A, \
                             CPR_PARAMETERS_OP_1ARGS,   \
                             CPR_PARAMETERS_OP_0ARGS)   \
    (CPR_ADD_PARAMETER, ##__VA_ARGS__)

#define CPR_PARAMETERS_EXPAND_CPR_ADD_PARAMETER_B(...)  \
    CPR_PARAMETERS_CHOOSE_OP(CPR_PARAMETERS_OP_0ARGS,   \
                             ##__VA_ARGS__,             \
                             CPR_PARAMETERS_OP_2ARGS_B, \
                             CPR_PARAMETERS_OP_1ARGS,   \
                             CPR_PARAMETERS_OP_0ARGS)   \
    (CPR_ADD_PARAMETER, ##__VA_ARGS__)

#define ENABLE_TO_CPR_PARAMETERS(type, ...)                          \
    friend void to_query_string(const type& m, cpr::Parameters& p) { \
        CPR_PARAMETERS_EXPAND_CPR_ADD_PARAMETER_A __VA_ARGS__()      \
    }

TEST_CASE("stringify") {
    using std::string_view_literals::operator""sv;

    SUBCASE("for string types") {
        constexpr const char s1[] = "test";
        CHECK_EQ(stringify_member(s1), "test"sv);

        CHECK_EQ("test", "test"sv);

        const std::string s{"hello"};
        CHECK_EQ(stringify_member(s), "hello"sv);
    }

    SUBCASE("for integrals") {
        int i = 42;
        CHECK_EQ(stringify_member(i), "42"sv);

        long l = 42;
        CHECK_EQ(stringify_member(l), "42"sv);

        double d = 42.0;
        CHECK_EQ(stringify_member(d), "42.000000"sv);
    }
}

struct foo {
    int i;
    std::string s;

    ENABLE_TO_CPR_PARAMETERS(foo,
                             (s, "str")(i, "n"))
};

TEST_CASE("to cpr parameters") {
    using std::string_view_literals::operator""sv;

    cpr::Parameters params;
    foo f{42, "hello world"};
    to_query_string(f, params);
    cpr::Session ss;
    auto qs = params.GetContent(*ss.GetCurlHolder());
    CHECK_EQ(qs, "str=hello%20world&n=42"sv);
}

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
