//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "doctest/doctest.h"

#include "lumper/cgroups/util.h"

namespace {

namespace cgroups = lumper::cgroups;

TEST_SUITE_BEGIN("cgroups/util");

TEST_CASE("mount point for subsystems") {
    SUBCASE("found for memory") {
        auto mp = cgroups::find_mount_point("memory");
        CHECK_FALSE(mp.empty());
    }

    SUBCASE("found for cpu") {
        auto mp = cgroups::find_mount_point("cpu");
        CHECK_FALSE(mp.empty());
    }

    SUBCASE("found for cpuset") {
        auto mp = cgroups::find_mount_point("cpuset");
        CHECK_FALSE(mp.empty());
    }
}

TEST_CASE("empty result for unknown or invalid subsystem") {
    auto mp = cgroups::find_mount_point("testing");
    CHECK(mp.empty());
}

TEST_CASE("throws when no cgroup path and no auto-create") {
    constexpr char mem_subsys[] = "memory";
    constexpr char name[] = "cgroup-test";
    REQUIRE_THROWS_AS( // NOLINT(readability-else-after-return)
            {
                auto _ = cgroups::get_cgroup_path_for_subsystem(mem_subsys, name, false);
            },
            std::filesystem::filesystem_error);

    SUBCASE("cause should be no entry") {
        try {
            auto _ = cgroups::get_cgroup_path_for_subsystem(mem_subsys, name, false);
        } catch (const std::filesystem::filesystem_error& e) {
            CAPTURE(e.what());
            CAPTURE(e.code());
            CHECK(e.code() == std::errc::no_such_file_or_directory);
        }
    }
}

TEST_CASE("throws when no mount point can be found") {
    constexpr char inv_subsys[] = "test";
    constexpr char name[] = "cgroup-test";
    CHECK_THROWS_AS( // NOLINT(readability-else-after-return)
            { auto _ = cgroups::get_cgroup_path_for_subsystem(inv_subsys, name, false); },
            std::runtime_error);
}

TEST_CASE("throws when no enough permission for creation") {
    constexpr char mem_subsys[] = "memory";
    constexpr char name[] = "cgroup-test";
    REQUIRE_THROWS_AS( // NOLINT(readability-else-after-return)
            {
                auto _ = cgroups::get_cgroup_path_for_subsystem(mem_subsys, name, true);
            },
            std::filesystem::filesystem_error);

    SUBCASE("cause should be no permission") {
        try {
            auto _ = cgroups::get_cgroup_path_for_subsystem(mem_subsys, name, true);
        } catch (const std::filesystem::filesystem_error& e) {
            CAPTURE(e.what());
            CAPTURE(e.code());
            CHECK(e.code() == std::errc::permission_denied);
        }
    }
}

TEST_SUITE_END();

} // namespace
