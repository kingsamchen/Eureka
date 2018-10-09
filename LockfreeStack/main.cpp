
#include <iostream>

#include "lstack.h"

int main()
{
    LStack<int> ls(5);
    ls.Push(1);
    ls.Push(3);
    ls.Push(5);
    ls.Pop();
    ls.Pop();
    ls.Pop();
    auto rv = ls.Pop();
    std::cout << rv.first;
    return 0;
}

