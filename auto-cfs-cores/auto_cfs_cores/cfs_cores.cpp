/*
 @ 0xCCCCCCCC
*/

#include "auto_cfs_cores/cfs_cores.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "kbase/basic_macros.h"
#include "kbase/path.h"
#include "kbase/string_util.h"
#include "kbase/tokenizer.h"

std::optional<std::string> QueryCGroupCPUPathForProc(const std::string& proc_path)
{
    std::ifstream in(proc_path);

    for (std::string line; std::getline(in, line);) {
        std::vector<std::string> fields;
        auto cnt = kbase::SplitString(line, ":", fields);
        constexpr size_t kFieldCnt = 3;
        if (cnt < kFieldCnt) {
            continue;
        }

        kbase::Tokenizer subsys(fields[1], ",");
        if (std::find(subsys.begin(), subsys.end(), "cpu") != subsys.end()) {
            return fields[2];
        }
    }

    return {};
}

std::optional<MountInfo> QueryMountInfoForProc(const std::string& proc_path)
{
    std::ifstream in(proc_path);
    for (std::string line; std::getline(in, line);) {
        std::vector<std::string> fields;
        auto cnt = kbase::SplitString(line, " ", fields);
        constexpr size_t kMinFieldCnt = 11;
        if (cnt < kMinFieldCnt) {
            continue;
        }

        auto it_opt = std::find(fields.cbegin(), fields.cend(), "-");
        if (it_opt == fields.cend()) {
            continue;
        }

        if (*std::next(it_opt) == "cgroup") {
            kbase::Tokenizer subsys(*std::next(it_opt, 3), ",");
            if (std::find(subsys.begin(), subsys.end(), "cpu") != subsys.end()) {
                return MountInfo{
                    fields[3], fields[4]
                };
            }
        }
    }

    return {};
}

std::optional<int> ReadFileFirstLineAsInt(const std::string& path)
{
    std::ifstream in(path);
    int value;
    in >> value;
    if (in.bad() || in.fail()) {
        return std::nullopt;
    }

    return value;
}

int ReadConfiguredCores()
{
    constexpr int kCoresUnavailable = -1;

    auto cpu_cgroup_pathname = QueryCGroupCPUPathForProc("/proc/self/cgroup");
    auto mount_info = QueryMountInfoForProc("/proc/self/mountinfo");
    if (!cpu_cgroup_pathname || !mount_info) {
        std::cerr << "Failed to obtain cpu cgroup pathname or mount info\n";
        return kCoresUnavailable;
    }

    kbase::Path cgroup_dir(mount_info->mount_point);
    kbase::Path mount_root(mount_info->root);
    kbase::Path cpu_pathname(*cpu_cgroup_pathname);
    if (mount_root != cpu_pathname) {
        if (!mount_root.AppendRelativePath(cpu_pathname, &cgroup_dir)) {
            std::cerr << "Failed to compose cgroup file path\n"
                      << "root=" << mount_root.value() << "\npathname=" << cpu_pathname.value()
                      << "mount-point=" << cgroup_dir.value();

            return kCoresUnavailable;
        }
    }

    auto quota_file = cgroup_dir.AppendWith("cpu.cfs_quota_us");
    auto quota = ReadFileFirstLineAsInt(quota_file.value());
    if (!quota) {
        std::cerr << "Failed to read quota file; path=" << quota_file.value();
        return kCoresUnavailable;
    }

    if (*quota == -1) {
        std::cout << "cfs cores does not apply!\n";
        return kCoresUnavailable;
    }

    auto period_file = cgroup_dir.AppendWith("cpu.cfs_period_us");
    auto period = ReadFileFirstLineAsInt(period_file.value());
    if (!period) {
        std::cerr << "Failed to read period file; path=" << period_file.value();
        return kCoresUnavailable;
    }

    return *quota / *period;
}
