/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <vector>

#include "infix_op.h"

int main()
{
    auto find = Infix([](const auto& container, const auto& value) {
        return std::find(container.begin(), container.end(), value);
    });

    auto seq = std::vector<int> { 1, 3, 5, 7, 9 };

    bool found = (seq | find | 7) != seq.cend();
    std::cout << found << std::endl;

    return 0;
}
