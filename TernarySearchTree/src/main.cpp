/*
 @ 0xCCCCCCCC
*/

#include "ternary_search_tree.h"

void TestInsertion()
{
    TernaryTree ternary_tree;
    ternary_tree.Insert("hello");
    ternary_tree.Insert("here");
    ternary_tree.Insert("world");
}

int main()
{
    TestInsertion();
    return 0;
}