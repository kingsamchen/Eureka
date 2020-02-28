#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 使用栈辅助实现二叉树的中序遍历
// 相关题目：binary_tree_inorder_traversal_q94.cpp
class BSTIterator {
public:
    vector<TreeNode*> pending_;

    BSTIterator(TreeNode* root) {
        while (root) {
            pending_.push_back(root);
            root = root->left;
        }
    }

    /** @return the next smallest number */
    int next() {
        auto node = pending_.back();
        pending_.pop_back();
        if (node->right) {
            for (auto p = node->right; p; p = p->left) {
                pending_.push_back(p);
            }
        }
        return node->val;
    }

    /** @return whether we have a next smallest number */
    bool hasNext() {
        return !pending_.empty();
    }
};
