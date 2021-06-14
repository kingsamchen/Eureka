#include "catch2/catch.hpp"

#include "scopedhandle/scoped_handle.h"

#include <cstdio>
#include <string>

namespace scoped {

TEST_CASE("For C FILE", "[FILE]") {
    scoped_file file(std::fopen("test.txt", "w"));
    REQUIRE(sizeof(file) == sizeof(void*));
    REQUIRE(!!file);

    std::string msg = "this is a test text";
    auto bytes_written = fwrite(msg.data(), sizeof(char), msg.size(), file.get());
    CHECK(bytes_written == msg.size());
    auto rc = fflush(file.get());
    CHECK(rc == 0);

    file = nullptr;
    REQUIRE(!file);
}

#if defined(_WIN32) || defined(_WIN64)

TEST_CASE("For Windows Handle", "[windows-handle]") {
    scoped_win_handle handle;
    REQUIRE(!handle);

    SECTION("equivalent to one that being initialized with INVALID_HANDLE_VALUE") {
        scoped_win_handle invalid_handle(INVALID_HANDLE_VALUE);
        CHECK(handle != invalid_handle);
        CHECK((!handle && !invalid_handle));
    }

    SECTION("unique_ptr abilities") {
        handle.reset(CreateEventW(nullptr, TRUE, FALSE, L"test_ev"));
        REQUIRE(!!handle);

        auto dup = std::move(handle);
        CHECK(!handle);
        CHECK(!!dup);
    } // the Event kernel object is destroyed at here.

    handle.reset(OpenEventW(EVENT_ALL_ACCESS, FALSE, L"test_ev"));
    CHECK(!handle);
    CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);
}

#endif

} // namespace scoped
