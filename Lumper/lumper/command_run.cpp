//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "lumper/commands.h"

#include <cstdint>

#include <sched.h>
#include <sys/mount.h>

#include "esl/scope_guard.h"
#include "esl/strings.h"
#include "fmt/ranges.h"
#include "spdlog/spdlog.h"

#include "base/exception.h"
#include "base/ignore.h"
#include "base/subprocess.h"
#include "lumper/cgroups/cgroup_manager.h"

namespace lumper {

struct mount_proc_before_exec : public base::subprocess::evil_pre_exec_callback {
    int run() noexcept override {
        // See https://man7.org/linux/man-pages/man7/mount_namespaces.7.html#NOTES
        if (::mount("", "/", "", MS_PRIVATE | MS_REC, "") != 0) {
            return errno;
        }

        std::uint64_t flags = MS_NOEXEC | MS_NOSUID | MS_NODEV;
        if (::mount("proc", "/proc", "proc", flags, "") == 0) {
            return 0;
        }
        return errno;
    }
};

void process(cli::cmd_run_t) {
    const auto& parser = cli::for_current_process().command_parser();

    base::subprocess::options opts;
    opts.clone_with_flags(CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWNET | CLONE_NEWIPC);
    if (!parser.get<bool>("--it")) {
        opts.set_stdin(base::subprocess::use_null);
        opts.set_stdout(base::subprocess::use_null);
        opts.set_stderr(base::subprocess::use_null);
    }

    mount_proc_before_exec mount_proc;
    opts.set_evil_pre_exec_callback(&mount_proc);

    cgroups::resource_config res_cfg;

    auto mem_limit = parser.present<std::string>("--memory");
    if (mem_limit) {
        res_cfg.set_memory_limit(*mem_limit);
    }

    auto argv = parser.present<std::vector<std::string>>("CMD");
    if (!argv || argv->empty()) {
        throw cli_parse_failure("No CMD provided", &parser);
    }

    SPDLOG_INFO("Prepare to run cmd: {}", *argv);
    try {
        cgroups::cgroup_manager cgroup_mgr("lumper-cgroup", res_cfg);

        base::subprocess proc(*argv, opts);
        ESL_ON_SCOPE_EXIT {
            base::ignore_unused(proc.wait());
            // NOLINTNEXTLINE(bugprone-lambda-function-name)
            SPDLOG_INFO("Command {} completed", esl::strings::join(*argv, " "));
        };

        cgroup_mgr.apply(proc.pid());
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("Failed to run cmd in sub-process; cmd={}", *argv);
        throw command_run_error(ex.what());
    }
}

} // namespace lumper
