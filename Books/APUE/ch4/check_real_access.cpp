#include <unistd.h>

#include <iostream>

int main(int argc, const char* argv[])
{
    if (argc !=2 ) {
        std::cout << "Error command!\n";
        return 1;
    }

    const char* file_path = argv[1];
    if (access(file_path, R_OK) == 0) {
        std::cout << "Read access granted\n";
    } else {
        std::cout << "Read access prohibited\n";
    }

    if (access(file_path, W_OK) == 0) {
        std::cout << "Write access granted\n";
    } else {
        std::cout << "Write access prohibited\n";
    }

    if (access(file_path, X_OK) == 0) {
        std::cout << "Execute access granted\n";
    } else {
        std::cout << "Execute access prohibited\n";
    }

    return 0;
}
