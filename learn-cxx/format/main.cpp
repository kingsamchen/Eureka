
#include <string>

#include <fmt/format.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

namespace {

std::string format_string() {
    return fmt::format("hello {} {}", 42);
}

} // namespace

TEST_CASE("format specifier must be compile-time") {
    static constexpr char fmt[] = "hello {}";
    CHECK_EQ("hello 42", fmt::format(fmt, 42));
}
