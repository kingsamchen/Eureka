#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void MyHandler(int sig)
{
    static int beeps = 0;
    printf("Beep\n");
    if (++beeps < 5) {
        alarm(1);
    } else {
        printf("BOOM!\n");
        exit(0);
    }
}


int main()
{
    signal(SIGALRM, MyHandler);

    alarm(1);

    while (1) {
        printf("in loop\n");
        sleep(10);
    }

    return 0;
}
