#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int sig)
{
    pid_t pid;

    while ((pid = waitpid(-1, NULL, 0)) > 0) {
        printf("Handler reaped child %d\n", (int)pid);
    }

    if (errno != ECHILD) {
        fputs("waitpid error", stderr);
    }

    sleep(2);

    return;
}

#define MAXBUF 255

int main()
{
    int i, n;
    char buf[MAXBUF];

    if (signal(SIGCHLD, handler) == SIG_ERR) {
        fputs("install signal handler error", stderr);
    }

    for (i = 0; i < 3; ++i) {
        if (fork() == 0) {
            printf("hello from child %d\n", (int)getpid());
            sleep(1);
            exit(0);
        }
    }

    if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
        fputs("install signal handler error", stderr);
    }

    printf("Parent processing input\n");

    while (1)
        ;

    exit(0);
}

