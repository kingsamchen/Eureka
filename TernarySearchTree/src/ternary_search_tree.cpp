/*
 @ 0xCCCCCCCC
*/

#include "ternary_search_tree.h"

#include <cassert>

#include "alphabet.h"

struct Node {
    Node()
    {}

    explicit Node(size_t index)
        : char_index(index)
    {}

    static constexpr size_t kLeft = 0;
    static constexpr size_t kMiddle = 1;
    static constexpr size_t kRight = 2;

    size_t char_index { Alphabet::npos };
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

// Returns the pointer to the child specified by `child_id` of the given `node`.
// Creates a child node if it wasn't there.
Node* TouchNodeChild(Node* node, size_t child_id, size_t char_index)
{
    assert(node != nullptr);
    Node*& child = node->children[child_id];
    if (!child) {
        child = new Node(char_index);
    }

    return child;
}

}   // namespace

TernaryTree::TernaryTree()
    : root_(new Node())
{}

TernaryTree::~TernaryTree()
{
    DestroyTree(root_);
    root_ = nullptr;
}

// Loop invariant: after checking each character of the word, we have reached
// the node on the path that matches the checked character sequence.
// ReSharper disable once CppMemberFunctionMayBeConst
void TernaryTree::Insert(const std::string& word)
{
    Node* node = root_;
    for (char ch : word) {
        auto index = Alphabet::GetCharacterIndex(ch);
        if (index == Alphabet::npos) {
            throw std::invalid_argument("word contains invalid character");
        }

        Node* next_node = TouchNodeChild(node, Node::kMiddle, index);
        while (index != next_node->char_index) {
            if (index < next_node->char_index) {
                next_node = TouchNodeChild(next_node, Node::kLeft, index);
            } else {
                next_node = TouchNodeChild(next_node, Node::kRight, index);
            }
        }

        node = next_node;
    }

    node->end_word = true;
}