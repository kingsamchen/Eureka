//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include <iostream>

#include "topt/topt.h"

int main() {
    constexpr std::string_view key = "ff5t 5xlu 4ij5 nm4p tkh7 enul w3uw 2pub";
    std::cout << topt::generate_topt(key);
    return 0;
}
