#include "rcp/rcp.h"

#include <cstdio>
#include <system_error>

#include <sys/wait.h>
#include <unistd.h>

namespace rcp {

// For systems like Linux, we should
// 1) use pipe() to create two pairs of pipes
// 2) fork() with a child process
// 3) inside new process, use dup() to install provided pipe fds into standard fd
// 4) run exec to load specific program's image

child_process run_child_process(const std::string& pathname, const std::vector<std::string>& args) {
    // From child's perspective.
    pipe std_in = make_pipe();
    pipe std_out = make_pipe();

    auto cpid = fork();
    if (cpid == -1) {
        throw std::system_error(std::error_code(errno, std::system_category()),
                                "fork() failure");
    }

    // Parent process.
    if (cpid != 0) {
        std_out.write_end() = nullptr;
        std_in.read_end() = nullptr;
        return {cpid, std::move(std_in), std::move(std_out)};
    }

    // Ignore potential errors.
    ::dup2(std_in.read_end().get(), STDIN_FILENO);
    ::dup2(std_out.write_end().get(), STDOUT_FILENO);
    std_out.reset();
    std_in.reset();

    std::vector<const char*> argv{pathname.c_str()};
    for (const auto& arg : args) {
        argv.push_back(arg.c_str());
    }
    argv.push_back(nullptr);
    auto argvp = const_cast<char* const*>(argv.data());
    if (::execvp(pathname.c_str(), argvp) < 0) {
        std::fprintf(stderr, "Failed to execl; errno=%d pathname=%s", errno, pathname.c_str());
        ::exit(EXIT_FAILURE);
    }

    // Should be unreachable.
    return {};
}

void child_process::join() {
    [[maybe_unused]] int status;
    ::waitpid(cpid, &status, 0);
}

} // namespace rcp
