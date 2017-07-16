#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void my_handler(int sig)
{
    printf("Caught SIGINT(%d)\n", sig);
    exit(0);
}

int main()
{
    if (signal(SIGINT, my_handler) == SIG_ERR) {
        fputs("signal error", stderr);
    }

    printf("we now wait for signal!");
    fflush(stdout);
    pause();

    exit(0);
}
