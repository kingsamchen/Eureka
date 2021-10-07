// Read one line from stdin and transform into upper case then write to stdout

#include <algorithm>
#include <iostream>
#include <string>

int main() {
    for (std::string line; std::getline(std::cin, line);) {
        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        });
        std::cout << line;
        std::cout.flush();
    }
    return 0;
}
