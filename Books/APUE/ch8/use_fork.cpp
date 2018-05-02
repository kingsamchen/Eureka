#include <unistd.h>
#include <iostream>

int g_var = 6;
char buf[] = "a write to stdout\n";

int main()
{
    int var = 88;
    pid_t pid;

    std::cout << "before fork\n"; 
    std::cout << "addr of g_var " << &g_var << "; addr of buf " << &buf << "\n";

    if ((pid = fork()) < 0) {
        std::cerr << "fork error!" << std::endl;
    } else if (pid == 0) {
        std::cout << "in child: addr of g_var " << &g_var << "; addr of buf " << &buf << "\n";
        g_var++;
        var++;
    } else {
        sleep(2);
    }

    std::cout << "pid " << getpid() << ", g_var " << g_var << ", var " << var << "\n";
    std::cout << "final: addr of g_var " << &g_var << "; addr of buf " << &buf << "\n";

    return 0;
}
