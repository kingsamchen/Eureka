/*
 @ 0xCCCCCCCC
*/

#include "trie.h"

#include <cassert>
#include <vector>

#include "alphabet.h"

struct TrieNode {
    TrieNode() = default;

    ~TrieNode() = default;

    char character { 0 };
    bool last_char_of_word { false };
    std::vector<TrieNode*> children { Alphabet::length(), nullptr };
};

namespace {

void DestroyTrie(TrieNode* node) noexcept
{
    assert(node != nullptr);
    for (TrieNode* child : node->children) {
        if (child) {
            DestroyTrie(child);
        }
    }

    delete node;
}

}   // namespace

Trie::Trie()
    : header_(new TrieNode())
{}

Trie::~Trie()
{
    DestroyTrie(header_);
    header_ = nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Trie::Insert(const std::string& word)
{
    TrieNode* node = header_;
    for (char ch : word) {
        auto pos = Alphabet::GetCharacterIndex(ch);
        if (pos == Alphabet::npos) {
            throw std::invalid_argument("word contains illegal character!");
        }

        TrieNode*& sub = node->children[pos];
        if (!sub) {
            sub = new TrieNode();
            sub->character = ch;
        }

        node = sub;
    }

    node->last_char_of_word = true;
}

bool Trie::Contains(const std::string& word) const
{
    const TrieNode* node = header_;
    for (char ch : word) {
        auto pos = Alphabet::GetCharacterIndex(ch);
        if (pos == Alphabet::npos) {
            return false;
        }

        const TrieNode* child = node->children[pos];
        if (!child) {
            return false;
        }

        node = child;
    }

    return node->last_char_of_word;
}