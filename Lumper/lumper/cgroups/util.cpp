//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "lumper/cgroups/util.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <system_error>
#include <vector>

#include <sys/stat.h>

#include "fmt/format.h"

#include "esl/strings.h"

namespace lumper::cgroups {

std::string find_mount_point(std::string_view subsystem) {
    std::ifstream in("/proc/self/mountinfo");
    if (!in) {
        throw std::filesystem::filesystem_error(
                "cannot open '/proc/self/mountinfo' file",
                std::error_code(errno, std::system_category()));
    }

    for (std::string line; std::getline(in, line);) {
        auto fields = esl::strings::split(line, ' ', esl::strings::skip_empty{})
                              .to<std::vector<std::string_view>>();
        assert(fields.size() > 4);
        auto toks = esl::strings::split(fields.back(), ',');
        if (std::find(toks.begin(), toks.end(), subsystem) != toks.end()) {
            return std::string(fields[4]);
        }
    }

    return {};
}

std::filesystem::path get_cgroup_path_for_subsystem(std::string_view subsystem,
                                                    std::string_view cgroup_name,
                                                    bool auto_create) {
    std::filesystem::path cgroup_root(find_mount_point(subsystem));
    if (cgroup_root.empty()) {
        throw std::runtime_error(
                fmt::format("mount point not found for subsystem={}", subsystem));
    }

    auto cgroup_path = cgroup_root / cgroup_name;
    if (!std::filesystem::exists(cgroup_path)) {
        if (!auto_create) {
            throw std::filesystem::filesystem_error(
                    "cgroup path for the subsystem not found",
                    cgroup_path,
                    std::make_error_code(std::errc::no_such_file_or_directory));
        }

        constexpr auto perm = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
        static_assert(perm == 0755); // NOLINT(readability-magic-numbers)
        if (int rc = ::mkdir(cgroup_path.c_str(), perm); rc != 0 && errno != EEXIST) {
            throw std::filesystem::filesystem_error(
                    "cannot mkdir for cgroup path",
                    cgroup_path,
                    std::error_code(errno, std::system_category()));
        }
    }

    return cgroup_path;
}

} // namespace lumper::cgroups
