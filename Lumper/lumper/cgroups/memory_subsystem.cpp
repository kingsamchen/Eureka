//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "lumper/cgroups/subsystems.h"

#include <cassert>
#include <fstream>

#include <unistd.h>

#include "fmt/format.h"
#include "fmt/os.h"
#include "spdlog/spdlog.h"

#include "base/file_util.h"
#include "lumper/cgroups/util.h"

namespace lumper::cgroups {
namespace {

constexpr char limit_filename[] = "memory.limit_in_bytes";
constexpr char task_filename[] = "tasks";

} // namespace

memory_subsystem::memory_subsystem(std::string_view cgroup_name, std::string_view memory_limit) {
    assert(!cgroup_name.empty());
    assert(!memory_limit.empty());
    cgroup_path_ = get_cgroup_path_for_subsystem(memory_subsystem::name, cgroup_name, true);
    auto limit_path = cgroup_path_ / limit_filename;
    base::write_to_file(limit_path, memory_limit);
}

memory_subsystem::~memory_subsystem() {
    // See https://lists.linuxfoundation.org/pipermail/containers/2009-March/016518.html
    auto rc = ::rmdir(cgroup_path_.c_str());
    if (rc != 0 && errno != ENOENT) {
        SPDLOG_ERROR("Failed to cleanup cgroup memory subsystem; errno={} path={}",
                     errno, cgroup_path_.native());
    }
}

void memory_subsystem::apply(int pid) {
    auto task_path = cgroup_path_ / task_filename;
    base::write_to_file(task_path, fmt::to_string(pid));
}

} // namespace lumper::cgroups
