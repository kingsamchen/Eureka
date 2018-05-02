
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 256

int main(int argc, char* argv[])
{
    char buf[BUF_SIZE];
    ssize_t bytes_read = 0;
    while ((bytes_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        ssize_t bytes_written = write(STDOUT_FILENO, buf, bytes_read);
        if (bytes_read != bytes_written) {
            printf("write error!");
            return 1;
        }
    }

    if (bytes_read < 0) {
        printf("read error!");
    }

    return 0;
}
