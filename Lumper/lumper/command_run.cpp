//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "lumper/commands.h"

#include <cstdint>

#include <sched.h>
#include <sys/mount.h>

#include "fmt/ranges.h"
#include "spdlog/spdlog.h"

#include "base/exception.h"
#include "base/ignore.h"
#include "base/subprocess.h"

namespace lumper {

struct mount_proc_before_exec : public base::subprocess::evil_pre_exec_callback {
    int run() noexcept override {
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

    std::vector<std::string> argv = {parser.get<std::string>("CMD")};
    if (auto args = parser.present<std::vector<std::string>>("ARGS"); args) {
        argv.insert(argv.end(), args->begin(), args->end());
    }

    SPDLOG_INFO("Prepare to run cmd: {}", argv);

    try {
        base::subprocess proc(argv, opts);
        base::ignore_unused(proc.wait());
    } catch (const std::exception& ex) {
        base::rethrow_as<command_run_error>(
                std::current_exception(),
                fmt::format("Failed to run cmd in sub-process; cmd={}", argv));
    }
}

} // namespace lumper
