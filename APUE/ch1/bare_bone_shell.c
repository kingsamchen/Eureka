#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 80

int main()
{
    char buf[MAX_LINE] = {0};

    printf("-> ");
    while (fgets(buf, MAX_LINE, stdin) != NULL) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("failed to spawn a new process\n");
            continue;
        } else if (pid == 0) {
            size_t len = strlen(buf);
            if (buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
            }

            execlp(buf, buf, (char*)0);
            printf("failed to exec a given command\n");
            return 1;
        }

        int status = 0;
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            printf("waitpid error!");
            return 1;
        }

        printf("-> ");
    }

    return 0;
}