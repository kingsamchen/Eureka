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

void SearchWord(Node* node, std::vector<std::string>* words, std::vector<char>* seq,
                const std::string& prefix)
{
    seq->push_back(Alphabet::GetCharacter(node->char_index));
    if (node->end_word) {
        std::string word(prefix);
        word.append(seq->begin(), seq->end());
        words->push_back(std::move(word));
    }

    if (node->children[Node::kMiddle]) {
        SearchWord(node->children[Node::kMiddle], words, seq, prefix);
    }

    seq->pop_back();

    if (node->children[Node::kLeft]) {
        SearchWord(node->children[Node::kLeft], words, seq, prefix);
    }

    if (node->children[Node::kRight]) {
        SearchWord(node->children[Node::kRight], words, seq, prefix);
    }
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

bool TernaryTree::Contains(const std::string& word) const
{
    const Node* node = root_;
    for (char ch : word) {
        auto index = Alphabet::GetCharacterIndex(ch);
        if (index == Alphabet::npos) {
            return false;
        }

        const Node* next_node = node->children[Node::kMiddle];
        if (!next_node) {
            return false;
        }

        while (index != next_node->char_index) {
            if (index < next_node->char_index) {
                next_node = next_node->children[Node::kLeft];
            } else {
                next_node = next_node->children[Node::kRight];
            }

            if (!next_node) {
                return false;
            }
        }

        node = next_node;
    }

    return node->end_word;
}

std::vector<std::string> TernaryTree::Search(const std::string& prefix) const
{
    std::vector<std::string> words;

    // Locate the node matching the last character of `prefix`.
    const Node* node = root_;
    for (char ch : prefix) {
        auto index = Alphabet::GetCharacterIndex(ch);
        if (index == Alphabet::npos) {
            return words;
        }

        const Node* next_node = node->children[Node::kMiddle];
        if (!next_node) {
            return words;
        }

        while (index != next_node->char_index) {
            if (index < next_node->char_index) {
                next_node = next_node->children[Node::kLeft];
            } else {
                next_node = next_node->children[Node::kRight];
            }

            if (!next_node) {
                return words;
            }
        }

        node = next_node;
    }

    if (node->end_word) {
        words.push_back(prefix);
    }

    std::vector<char> sequence;
    if (node->children[Node::kMiddle]) {
        SearchWord(node->children[Node::kMiddle], &words, &sequence, prefix);
    }

    return words;
}