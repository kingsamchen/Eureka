#include <string>

using namespace std;

struct Node {
    Node* child[26] {nullptr};
    bool is_word_ = false;
};

// 核心思路
// 直接采用最直接的trie实现即可
// Node 定义是核心
class Trie {
public:
    Node* root_;

    /** Initialize your data structure here. */
    Trie() {
        root_ = new Node();
    }

    /** Inserts a word into the trie. */
    void insert(string word) {
        auto node = root_;
        for (auto c : word) {
            auto idx = c - 'a';
            if (!node->child[idx]) {
                node->child[idx] = new Node();
            }
            node = node->child[idx];
        }

        node->is_word_ = true;
    }

    /** Returns if the word is in the trie. */
    bool search(string word) {
        auto node = root_;
        for (auto c : word) {
            auto idx = c - 'a';
            if (!node->child[idx]) {
                return false;
            }
            node = node->child[idx];
        }

        return node->is_word_;
    }

    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        auto node = root_;
        for (auto c : prefix) {
            auto idx = c - 'a';
            if (!node->child[idx]) {
                return false;
            }
            node = node->child[idx];
        }

        return true;
    }
};
