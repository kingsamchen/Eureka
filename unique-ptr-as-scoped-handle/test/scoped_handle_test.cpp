#include "catch2/catch.hpp"

#include "scopedhandle/scoped_handle.h"

#include <cstdio>
#include <string>

namespace scoped {

TEST_CASE("For C FILE", "[FILE]") {
    scoped_file file(std::fopen("test.txt", "w"));
    REQUIRE(!!file);

    std::string msg = "this is a test text";
    auto bytes_written = fwrite(msg.data(), sizeof(char), msg.size(), file.get());
    CHECK(bytes_written == msg.size());
    auto rc = fflush(file.get());
    CHECK(rc == 0);

    file = nullptr;
    REQUIRE(!file);
}

} // namespace scoped
