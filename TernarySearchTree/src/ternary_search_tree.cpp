/*
 @ 0xCCCCCCCC
*/

#include "ternary_search_tree.h"

#include <cassert>

struct Node {
    static constexpr size_t kLeft = 0;
    static constexpr size_t kMiddle = 1;
    static constexpr size_t kRight = 2;

    char character { 0 };
    bool end_word { false };
    Node* children[3] { nullptr };
};

namespace {

void DestroyTree(Node* node) noexcept
{
    assert(node != nullptr);
    for (auto child : node->children) {
        if (child) {
            DestroyTree(child);
        }
    }

    delete node;
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

void TernaryTree::Insert(const std::string& word)
{
    for (char ch : word) {

    }
}