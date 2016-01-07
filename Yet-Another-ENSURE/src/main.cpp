/*
 @ 0xCCCCCCCC
*/

#include <conio.h>

#include <vector>

#include "ensure.h"

int main()
{
    std::vector<int> v {1, 3, 5};
    ENSURE(CHECK, v.empty())(v.size()).Require();
    ENSURE(RAISE, v.empty())(v.size()).Require();
    ENSURE(RAISE_WITH_DUMP, v.empty())(v.size()).Require();
    _getch();
    return 0;
}