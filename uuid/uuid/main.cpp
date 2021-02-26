#include <iostream>

#include "uuid/uuid.h"

int main()
{
    for (int i = 0; i < 10; ++i) {
        auto id = uuid::uuid::make_v4(uuid::default_random_generator::instance());
        std::cout << id.string() << "\n";
    }
    return 0;
}
