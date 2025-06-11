//
// Kingsley Chen
//

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "cpo/cpo_member_function.h"
#include "cpo/cpo_tag_invoke.h"

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

namespace {

struct foo {};

struct bar {};

void tag_invoke(cpo::detail::print_impl_concept /*unused*/, bar& /*unused*/) {
    fmt::println("print for bar");
}

void tag_invoke(cpo::detail::print_impl_sfinae /*unused*/, bar& /*unused*/) {
    fmt::println("print for bar");
}

struct c_bar {};

void tag_invoke(cpo::detail::print_impl_sfinae /*unused*/, const c_bar& /*unused*/) {}

struct bar2 {
    void print() {
        fmt::println("print for bar2");
    }
};

} // namespace

TEST_CASE("is tag invocable") {
    static_assert(cpo::detail::is_tag_invocable_v<cpo::detail::print_impl_sfinae, bar&>);

    static_assert(cpo::detail::is_tag_invocable_v<cpo::detail::print_impl_sfinae, const c_bar&>);
}

TEST_CASE("apply cpo with tag_invoke") {
    foo f;
    bar b;
    cpo::print_concept(f);
    cpo::print_concept(b);
    fmt::println("---");
    cpo::print_sfinae(f);
    cpo::print_sfinae(b);
}

TEST_CASE("apply cpo with member function customization") {
    foo f;
    bar2 b;
    cpo2::print(f);
    cpo2::print(b);
}
