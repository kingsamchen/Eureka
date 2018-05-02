#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    if ((pid = fork()) == 0) {
        pause();    // wait for a signal to arrive.
        printf("control should never reach here!\n");
        exit(0);
    }

    kill(pid, SIGKILL);
    printf("The child process should have been killed!\n");
    return 0;
}
