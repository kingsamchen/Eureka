#include <unistd.h>

#include <cstdio>
#include <iostream>

void CharAtATime(const char* str)
{
    setbuf(stdout, nullptr);

    for (const char* ptr = str; *ptr != '\0'; ++ptr) {
        putc(*ptr, stdout);
    }
}

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        //std::cerr << "fork error" << std::endl;
    } else if (pid == 0) {
        CharAtATime("output from child\n");
    } else {
        CharAtATime("output from parent\n");
    }

    return 0;
}

