
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_MAX 64

const char buf[] = "this is a test text\n";

void open_or_create(const char* file_name)
{
    printf("trying to open file: %s\n", file_name);
    int fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("failed to open the file\n");
        return;
    }

    printf("now we are writing...\n");
    write(fd, buf, sizeof(buf));

    printf("\nnow close the file\n");
    close(fd);
}

void read_file(const char* file_name)
{
    int fd = open(file_name, O_RDONLY, S_IRUSR);
    if (fd == -1) {
        printf("failed to open the file %s", file_name);
        return;
    }

    char buf[BUF_MAX];
    ssize_t bytes_read = 0;
    while ((bytes_read = read(fd, buf, BUF_MAX)) != 0) {
        printf("%s", buf);
    }

    printf("\nnow close the file\n");
    close(fd);
}

int main()
{
    open_or_create("test.txt");
    read_file("test.txt");
    return 0;
}
