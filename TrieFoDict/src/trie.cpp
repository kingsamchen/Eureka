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

    size_t char_index { Alphabet::npos };
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