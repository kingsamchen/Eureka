//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef LUMPER_CGROUPS_UTIL_H_
#define LUMPER_CGROUPS_UTIL_H_

#include <filesystem>
#include <string>
#include <string_view>

namespace lumper::cgroups {

// Returns mount point of the given subsystem, otherwise returns empty string if not found.
// Throws `std::filesystem::filesystem_error` on file failure.
std::string find_mount_point(std::string_view subsystem);

// Returns the path to the desired cgroup.
// If the path doesn't exist but `auto_create` is true then create it automatically.
// Throws:
//  - `std::filesystem::filesystem_error` if path doesn't exist and `auto_create` is false; in
//    this particular case, error_code should correspond to `no_such_file_or_directory`.
//  - `std::filesystem::filesystem_error` also for other file operation failures.
//  - `std::runtime_error` if mount point of `subsystem` cannot be found.
std::filesystem::path get_cgroup_path_for_subsystem(std::string_view subsystem,
                                                    std::string_view cgroup_name,
                                                    bool auto_create);

} // namespace lumper::cgroups

#endif // LUMPER_CGROUPS_UTIL_H_
