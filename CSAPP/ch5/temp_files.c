#include <stdio.h>
#include <unistd.h>

#define MAX_SIZE 255

// GCC implies `tmpnam` is dangerous, use `mkstemp` instead...
const char* GenerateTmpFileName()
{
    return tmpnam(NULL);
}

void GenerateTmpFile()
{
    char path_buf[MAX_SIZE];
    char file_path[MAX_SIZE];
    ssize_t rv = 0;

    FILE* fp = tmpfile();
    sprintf(path_buf, "/proc/self/fd/%d", fileno(fp));
    readlink(path_buf, file_path, MAX_SIZE);
    if (rv == -1) {
        printf("Failed to read tmp file path");
        return;
    }

    printf("Create a tmp file at %s", file_path);
}

int main()
{
    printf("Temporary filename test\n");
    for (int i = 0; i < 3; ++i) {
        printf("-> %s\n", GenerateTmpFileName());
    }

    printf("\nCreate temp file test\n");
    GenerateTmpFile();
    
    return 0;
}
