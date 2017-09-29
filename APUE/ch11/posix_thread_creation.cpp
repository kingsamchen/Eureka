/*
 @ 0xCCCCCCCC
*/

#include <cstdio>

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

void DumpThreadInfo(const char* msg)
{
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    printf("%s pid %d tid %lu (0x%lx)\n", msg, pid, static_cast<unsigned long>(tid),
           static_cast<unsigned long>(tid));
}

void* ThreadFunc(void* arg)
{
    (void)arg;
    DumpThreadInfo("new thread: ");
    return nullptr;
}

int main()
{
    pthread_t new_tid;
    int err = pthread_create(&new_tid, nullptr, ThreadFunc, nullptr);
    if (err != 0) {
        fputs("Can't create thread!", stderr);
        _exit(1);
    }

    DumpThreadInfo("main thread: ");
    pthread_join(new_tid, nullptr);
    return 0;
}