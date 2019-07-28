/*
 @ 0xCCCCCCCC
*/

#include <cstdio>
#include <string>
#include <utility>

#include "kbase/command_line.h"
#include "kbase/file_iterator.h"
#include "kbase/file_util.h"
#include "kbase/path.h"
#include "kbase/string_format.h"
#include "kbase/string_util.h"

struct ControlParams {
    int max_past_mins;
    std::string exclude_pattern;
    bool recursive;
};

std::pair<kbase::Path, ControlParams> HandleCommandline()
{
    constexpr const char* kUsage =
        "Usage: list_modified [--time=10(mins) --exclude=foo*.exe --no-recursive] {dir-path}";

    auto cmdline = kbase::CommandLine::ForCurrentProcess();

    if (cmdline.HasSwitch("help") || cmdline.HasSwitch("h")) {
        printf("%s\n", kUsage);
        return {};
    }

    if (cmdline.parameter_count() == 0) {
        printf("Error: no dir-path given!\n%s\n", kUsage);
        return {};
    }

    auto past_time = cmdline.GetSwitchValueAs<int>("time", 10);
    auto exclude = cmdline.GetSwitchValue("exclude");
    auto recursive = !cmdline.HasSwitch("no-recursive");

    ControlParams params {past_time, exclude, recursive};

    return {kbase::Path::FromUTF8(cmdline.GetParameter(0)), std::move(params)};
}

std::string FileTimeToString(kbase::FileTime ft)
{
    auto time = kbase::TimePointToLocalTime(ft.value());
    const auto& tm = time.first;
    return kbase::StringPrintf("%04d-%02d-%02d %02d:%02d:%02d",
                               tm.tm_year + 1900,
                               tm.tm_mon + 1,
                               tm.tm_mday,
                               time.first.tm_hour,
                               time.first.tm_min,
                               time.first.tm_sec);
}

void ProcessResults(std::vector<kbase::FileInfo>& results)
{
    if (results.empty()) {
        return;
    }

    std::sort(results.begin(), results.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.last_modified_time().value() > rhs.last_modified_time().value();
    });

    for (const auto& info : results) {
        printf("%s %lldkb \t%s\n",
               FileTimeToString(info.last_modified_time()).c_str(),
               info.file_size() / 1024,
               info.file_path().AsUTF8().c_str());
    }

    results.clear();
}

void List(const kbase::Path& dir, const ControlParams& params)
{
    auto oldest = std::chrono::system_clock::now() - std::chrono::minutes(params.max_past_mins);
    auto excluded_pattern = kbase::ASCIIToWide(params.exclude_pattern);

    std::vector<kbase::FileInfo> lists;

    kbase::FileIterator file_it(dir, params.recursive);
    for (auto end = kbase::FileIterator(); file_it != end; ++file_it) {
        if (file_it->is_directory()) {
            continue;
        }

        auto too_long = file_it->last_modified_time().value() < oldest;
        auto excluded = kbase::MatchPattern(file_it->file_path().filename().value(), excluded_pattern);
        if (too_long || excluded) {
            continue;
        }

        lists.push_back(*file_it);
    }

    ProcessResults(lists);
}

// Usage: list_modified --time=mins --exclude=foo*.exe --no-recursive path-to-dir
// by default, --time=10 and both --exclude and --no-recursive are not set.
int main(int argc, char* argv[])
{
    kbase::CommandLine::Init(argc, argv);

    auto result = HandleCommandline();
    if (result.first.empty()) {
        return 0;
    }

    if (!kbase::DirectoryExists(result.first)) {
        printf("The directory specified does not exist!\n");
        return 0;
    }

    List(result.first, result.second);

    return 0;
}
