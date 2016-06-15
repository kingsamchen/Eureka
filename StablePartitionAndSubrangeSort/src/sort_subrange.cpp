/*
 @ 0xCCCCCCCC
*/

#include <algorithm>

template<typename RandomIt>
void SubrangeSort(RandomIt first, RandomIt last, RandomIt sub_first, RandomIt sub_last)
{
    if (sub_first == sub_last) {
        return;
    }

    if (sub_first != first) {
        std::nth_element(first, sub_first, last);
        ++sub_first;
    }

    std::partial_sort(sub_first, sub_last, last);
}