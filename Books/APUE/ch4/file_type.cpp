#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

int main()
{
    const char file_path[] = "test_file.txt";
    struct stat file_attr;
    int rv = stat(file_path, &file_attr);
    if (rv != 0) {
        std::cerr << "Failed to query file attribution\n";
    }

    bool regular_file = static_cast<bool>(S_ISREG(file_attr.st_mode));
    std::cout << "Is regular file: " << regular_file << std::endl;
    std::cout << "uid of the file: " << file_attr.st_uid;

    return 0;
}
