/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TRIE_FOR_DICT_TRIE_H_
#define TRIE_FOR_DICT_TRIE_H_

struct TrieNode;

class Trie {
public:
    Trie();

    ~Trie();

private:
    TrieNode* header_;
};

#endif  // TRIE_FOR_DICT_TRIE_H_