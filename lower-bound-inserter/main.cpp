
#include <iostream>
#include <vector>

#include "lower_bound_inserter.h"

void StandardCase()
{
    std::vector<int> major = {1, 3, 5, 7, 9};
    std::vector<int> sub = {3, 2, 4, 6};
    std::copy(sub.begin(), sub.end(), ext::lower_bound_inserter(major));
    std::copy(major.begin(), major.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void DecreaseOrderCase()
{
    std::vector<int> major{9, 7, 5, 3, 1};
    std::vector<int> sub{2, 4, 3, 6};
    auto greater = [](const auto& lhs, const auto& rhs) { return rhs < lhs; };
    std::copy(sub.begin(), sub.end(), ext::lower_bound_inserter(major, greater));
    std::copy(major.begin(), major.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

int main()
{
    StandardCase();
    DecreaseOrderCase();
    return 0;
}
