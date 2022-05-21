//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef LUMPER_CGROUPS_CGROUP_MANAGER_H_
#define LUMPER_CGROUPS_CGROUP_MANAGER_H_

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace lumper::cgroups {

class subsystem;

class resource_config {
public:
    resource_config& set_memory_limit(std::string_view limit) {
        memory_limit_ = limit;
        return *this;
    }

    resource_config& set_cpus(std::string_view cpus) {
        cpus_ = cpus;
        return *this;
    }

    resource_config& set_cpu_set(const std::string& cpu_set) {
        cpu_set_ = cpu_set;
        return *this;
    }

    const std::string& memory_limit() const noexcept {
        return memory_limit_;
    }

    const std::string& cpus() const noexcept {
        return cpus_;
    }

    const std::string& cpu_set() const noexcept {
        return cpu_set_;
    }

private:
    std::string memory_limit_;
    std::string cpus_;
    std::string cpu_set_;
};

class cgroup_manager {
public:
    // Throws
    //  - `std::filesystem::filesystem_error` for filesystem related errors.
    //  - `std::runtime_error` if mount point of `subsystem` cannot be found.
    cgroup_manager(std::string name, const resource_config& cfg);

    ~cgroup_manager();

    cgroup_manager(const cgroup_manager&) = delete;

    cgroup_manager(cgroup_manager&&) = delete;

    cgroup_manager& operator=(const cgroup_manager&) = delete;

    cgroup_manager& operator=(cgroup_manager&&) = delete;

    void apply(int pid);

private:
    std::string name_;
    std::vector<std::unique_ptr<subsystem>> subsystems_;
};

} // namespace lumper::cgroups

#endif // LUMPER_CGROUPS_CGROUP_MANAGER_H_
