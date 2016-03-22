/*
 @ 0xCCCCCCCC
*/

#include "ternary_search_tree.h"

struct Node {
    char character { 0 };
    bool end_word { false };
    Node* left_child { nullptr };
    Node* middle_child { nullptr };
    Node* right_child { nullptr };
};

namespace {

void DestroyTree(Node* root) noexcept
{
    // TODO:
}

}   // namespace

TernaryTree::TernaryTree()
    : root_(nullptr)
{}

TernaryTree::~TernaryTree()
{
    if (root_) {
        DestroyTree(root_);
        root_ = nullptr;
    }
}
