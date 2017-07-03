#include <unistd.h>
#include <iostream>

int g_var = 6;

int main()
{
    int var = 88;
    pid_t pid;

    std::cout << "before vfork\n";

    if ((pid = vfork()) < 0) {
        std::cerr << "vfork error" << std::endl;
    } else if (pid == 0) {
        g_var++;
        var++;
        _exit(0);
    }

    std::cout << "pid " << getpid() << " g_var " << g_var << " var " << var << "\n";
    
    return 0;
}
