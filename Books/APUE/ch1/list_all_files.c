
#include <dirent.h>
#include <stdio.h>

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        printf("usage: ls directory-name");
        return 1;
    }

    const char* dir_name = argv[1];
    DIR* dp = opendir(dir_name);
    if (dp == NULL) {
        printf("failed to open directory %s", dir_name);
        return 1;
    }

    struct dirent* dir_entry = NULL;
    while ((dir_entry = readdir(dp)) != NULL) {
        printf("%s\n", dir_entry->d_name);
    }

    closedir(dp);

    return 0;
}
