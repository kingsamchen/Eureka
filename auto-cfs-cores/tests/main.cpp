/*
 @ 0xCCCCCCCC
*/

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "auto_cfs_cores/cfs_cores.h"

TEST_CASE("Query cpu cgroup path", "[cfs_cores]")
{
    auto path = QueryCGroupCPUPathForProc("/proc/self/cgroup");
    REQUIRE(path.has_value());
    REQUIRE(!path->empty());

    SECTION("when file does not exist") {
        path = QueryCGroupCPUPathForProc("/proc/not-exist");
        REQUIRE_FALSE(path.has_value());
    }
}

TEST_CASE("Query mount-point info", "[cfs_cores]")
{
    auto info = QueryMountInfoForProc("/proc/self/mountinfo");
    REQUIRE(info.has_value());
    REQUIRE_FALSE((info->root.empty() || info->mount_point.empty()));

    SECTION("when file does not exist") {
        info = QueryMountInfoForProc("/proc/not-exist");
        REQUIRE_FALSE(info.has_value());
    }
}