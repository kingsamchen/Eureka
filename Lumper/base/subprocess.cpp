//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "base/subprocess.h"

#include <cstddef>
#include <stdexcept>
#include <system_error>

#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

namespace base {
namespace {

void check_system_error(int ret, const char* what) {
    if (ret == -1) {
        throw std::system_error(errno, std::system_category(), what);
    }
}

} // namespace

subprocess::subprocess(const std::vector<std::string>& argv, const options& opts) {
    if (argv.empty()) {
        throw std::invalid_argument("args cannot be empty");
    }

    std::unique_ptr<const char*[]> argvp(new const char*[argv.size() + 1]);
    for (size_t i = 0; i < argv.size(); ++i) {
        argvp[i] = argv[i].c_str();
    }
    argvp[argv.size()] = nullptr;

    spawn(std::move(argvp), opts);
}

void subprocess::spawn(std::unique_ptr<const char*[]> argvp, const options& opts) {
    // pre-clone process

    auto pid = static_cast<pid_t>(::syscall(SYS_clone, opts.clone_flags_, 0, nullptr, nullptr));
    check_system_error(pid, "failed to clone");

    if (pid == 0) {
        // within child process now.

        auto executable = argvp[0];
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
        auto argv = const_cast<char**>(argvp.get());
        auto ret = ::execvp(executable, argv);
        if (ret == -1) {
            std::fprintf(stderr, "Failed to execl; errno=%d executable=%s", errno, executable);
            ::exit(EXIT_FAILURE);
        }
    }

    // when we are done.
    pid_ = pid;
}

void subprocess::wait() {
    [[maybe_unused]] int status{};
    ::waitpid(pid_, &status, 0);
}

} // namespace base
