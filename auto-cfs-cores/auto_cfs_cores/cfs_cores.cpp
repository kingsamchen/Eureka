/*
 @ 0xCCCCCCCC
*/

#include "auto_cfs_cores/cfs_cores.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "kbase/basic_macros.h"
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

int ReadConfiguredCores()
{
    return -1;
}
