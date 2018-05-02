#include <unistd.h>
#include <sys/wait.h>

#include <iostream>

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        std::cerr << "fork error" << std::endl;
    } else if (pid == 0) {
        if ((pid = fork()) < 0) {
            std::cerr << "fork error" << std::endl;
        } else if (pid > 0) {
            // make direct child die
            exit(0);
        }

        sleep(2);
        std::cout << "grand child here, parent pid = " << getppid() << "\n";
        exit(0);
    }

    if (waitpid(pid, nullptr, 0) != pid) {
        std::cerr << "waitpid error" << std::endl;
    }

    return 0;
}

