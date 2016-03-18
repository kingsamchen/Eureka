/*
 @ 0xCCCCCCCC
*/

#include "trie.h"

#include <vector>

class TrieNode {
private:
    size_t char_index_;
    bool last_char_of_word_;
    std::vector<TrieNode*> descendents_;
};