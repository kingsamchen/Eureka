/*
 @ 0xCCCCCCCC
*/

#include "ternary_search_tree.h"

#include <cassert>
#include <iostream>

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

void TestSearch()
{
    TernaryTree ternary_tree;
    ternary_tree.Insert("hello");
    ternary_tree.Insert("here");
    ternary_tree.Insert("world");
    ternary_tree.Insert("he");

    auto words = ternary_tree.Search("he");
    for (auto&& word : words) {
        std::cout << word << std::endl;
    }
}

int main()
{
    TestSearch();
    return 0;
}