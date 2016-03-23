/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TERNARY_SEARCH_TREE_H_
#define TERNARY_SEARCH_TREE_H_

#include <string>
#include <vector>

struct Node;

class TernaryTree {
public:
    TernaryTree();

    ~TernaryTree();

    // Omit copy/move semantics here.

    void Insert(const std::string& word);

    bool Contains(const std::string& word);

    std::vector<std::string> Search(const std::string& prefix);

private:
    // We use `root_` as sentinel.
    Node* root_;
};

#endif  // TERNARY_SEARCH_TREE_H_