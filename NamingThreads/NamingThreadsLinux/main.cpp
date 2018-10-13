/*
 @ 0xCCCCCCCC
*/

#include <cstdio>
#include <thread>

#include <errno.h>
#include <unistd.h>

#include <sys/prctl.h>
#include <sys/syscall.h>

pid_t GetCurrentThreadID()
{
    return syscall(__NR_gettid);
}

void SetThreadName(const char* name)
{
    if (GetCurrentThreadID() == getpid()) {
        return;
    }

    int err = prctl(PR_SET_NAME, name);
    if (err < 0) {
        fprintf(stderr, "error: %d", errno);
    }
}


int main()
{
    std::thread th([] {
        SetThreadName("Worker");
        printf("Set thread name complete!\n");
    });

    th.join();

    return 0;
}
