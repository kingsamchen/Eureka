
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

int ksystem(const char* cmdstr)
{
    pid_t pid;
    int status = 0;

    if (cmdstr == nullptr) {
        return -1;
    }

    if ((pid = fork()) < 0) {
        std::cerr << "fork failed" << std::endl;
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmdstr, nullptr);
        std::cout << "child process shall never get here\n";
        _exit(127);
    } else {
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
    }

    return status;
}

int main(int, char* argv[])
{
    const char* cmd_str = argv[1];
    std::cout << "-> " << ksystem(cmd_str) << std::endl;
    return 0;
}

