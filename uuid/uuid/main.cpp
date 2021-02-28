#include <iostream>

#include "uuid/uuid.h"

int main()
{
    for (int i = 0; i < 10; ++i) {
        auto id = uuid::uuid::make_v1();
        std::cout << id.string() << "\n";
    }

    return 0;
}
