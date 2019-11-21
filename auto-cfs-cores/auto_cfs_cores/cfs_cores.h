/*
 @ 0xCCCCCCCC
*/

#ifndef CFS_CORES_H_
#define CFS_CORES_H_

#include <optional>
#include <string>

// For tests [
std::optional<std::string> QueryCGroupCPUPathForProc(const std::string& proc_path);

struct MountInfo {
    std::string root;
    std::string mount_point;
};

std::optional<MountInfo> QueryMountInfoForProc(const std::string& proc_path);

// ]

int ReadConfiguredCores();

#endif  // CFS_CORES_H_
