/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void RunClient(int pipe_wfd)
{
    std::string msg;
    while (true) {
        std::getline(std::cin, msg);
        if (msg == "exit") {
            std::cout << "[C] Bye!" << std::endl;
            return;
        } else {
            write(pipe_wfd, msg.data(), msg.length());
        }
    }
}

void RunServer(int pipe_rfd)
{
    const int kBufSize = 255;
    char buf[kBufSize];
    while (true) {
        auto bytes_read = read(pipe_rfd, buf, kBufSize);
        if (bytes_read == 0) {
            return;
        }

        std::cout << "[S] -> " << std::string(buf, bytes_read) << std::endl;
    }
}

int main()
{
    std::cout << "[S] server is online!" << std::endl;

    int pfd[2] {0};
    if (pipe(pfd) == -1) {
        std::cerr << "[S] creating pipe failed: " << errno << std::endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "[S] spawn child process failed: " << errno << std::endl;
        return 1;
    } else if (pid == 0) {
        close(pfd[0]);
        std::cout << "[C] client is online" << std::endl;
        RunClient(pfd[1]);
    } else {
        close(pfd[1]);
        RunServer(pfd[0]);
        waitpid(pid, nullptr, 0);
    }

    return 0;
}
