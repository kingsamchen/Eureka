#include <iostream>

#include "uuid/uuid.h"

int main()
{
    auto id = uuid::uuid::make_from("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
    constexpr std::string_view sv("www.widgets.com");
    auto idv3 = uuid::uuid::make_v3(id, sv);
    std::cout << idv3.string();
    return 0;
}
