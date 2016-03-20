/*
 @ 0xCCCCCCCC
*/

#include "trie.h"

#include <cassert>
#include <queue>
#include <utility>
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

        TrieNode*& child = node->children[pos];
        if (!child) {
            child = new TrieNode();
            child->character = ch;
        }

        node = child;
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

std::vector<std::string> Trie::Search(const std::string& prefix) const
{
    std::vector<std::string> words;

    // Locate the node matching the last character of `prefix`.
    const TrieNode* node = header_;
    for (char ch : prefix) {
        auto pos = Alphabet::GetCharacterIndex(ch);
        if (pos == Alphabet::npos) {
            return words;
        }

        const TrieNode* child = node->children[pos];
        if (!child) {
            return words;
        }

        node = child;
    }

    // We use `processing_nodes` as a stack to maintain nodes that are being processed.
    using CludeNode = std::pair<const TrieNode*, size_t>;
    std::vector<CludeNode> processing_nodes;
    processing_nodes.emplace_back(CludeNode(node, 0U));
    while (!processing_nodes.empty()) {
        // Don't access `current` after having pushed another node into `processing_nodes`.
        // Because the push might result in memory reallocation, rendering `current` an invalid
        // reference.
        auto& current = processing_nodes.back();
        bool child_node_found = false;
        while (current.second < current.first->children.size()) {
            const TrieNode* child = current.first->children[current.second];
            ++current.second;
            if (child) {
                processing_nodes.emplace_back(CludeNode(child, 0U));
                child_node_found = true;
                break;
            }
        }

        if (!child_node_found) {
            if (processing_nodes.back().first->last_char_of_word) {
                std::string word = prefix;
                std::for_each(processing_nodes.cbegin() + 1, processing_nodes.cend(),
                              [&word](const auto& clude_node) {
                    word += clude_node.first->character;
                });
                words.push_back(std::move(word));
            }

            processing_nodes.pop_back();
        }
    }

    return words;
}