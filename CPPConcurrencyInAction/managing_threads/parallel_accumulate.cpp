/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <vector>

#include "parallel_accumulate.h"

void TestParallelAccumulate()
{
    std::vector<int> ary;
    for (int i = 1; i <= 100; ++i) {
        ary.push_back(i);
    }

    auto result = parallel_accumulate(std::begin(ary), std::end(ary), 0);
    std::cout << result << std::endl;
}