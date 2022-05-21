//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "lumper/cgroups/cgroup_manager.h"

#include "spdlog/spdlog.h"

#include "lumper/cgroups/subsystems.h"

namespace lumper::cgroups {

cgroup_manager::cgroup_manager(std::string name, const resource_config& cfg)
    : name_(std::move(name)) {
    if (!cfg.memory_limit().empty()) {
        subsystems_.push_back(std::make_unique<memory_subsystem>(name_, cfg.memory_limit()));
    }

    SPDLOG_INFO("Enabled cgroup subsystems count={}", subsystems_.size());
}

cgroup_manager::~cgroup_manager() = default;

void cgroup_manager::apply(int pid) {
    // TODO(KC): Be tolerant for failure of one subsystem?
    for (auto& subsys : subsystems_) {
        subsys->apply(pid);
    }
}

} // namespace lumper::cgroups
