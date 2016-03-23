/*
 @ 0xCCCCCCCC
*/

#include "ternary_search_tree.h"

#include <cassert>

void TestInsertion()
{
    TernaryTree ternary_tree;
    ternary_tree.Insert("hello");
    ternary_tree.Insert("here");
    ternary_tree.Insert("world");
}

void TestContains()
{
    TernaryTree ternary_tree;
    ternary_tree.Insert("hello");
    ternary_tree.Insert("here");
    ternary_tree.Insert("world");

    assert(ternary_tree.Contains("world") == true);
    assert(ternary_tree.Contains("here") == true);
    assert(ternary_tree.Contains("hello") == true);

    assert(ternary_tree.Contains("a-b") == false);
    assert(ternary_tree.Contains("her") == false);
}

int main()
{
    TestContains();
    return 0;
}