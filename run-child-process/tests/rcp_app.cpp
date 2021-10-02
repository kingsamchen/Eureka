// Read one line from stdin and transform into upper case then write to stdout

#include <algorithm>
#include <iostream>
#include <string>

int main() {
    for (std::string line; std::getline(std::cin, line);) {
        std::transform(line.begin(), line.end(), line.begin(), toupper);
        std::cout << line << std::endl;
    }
    return 0;
}
