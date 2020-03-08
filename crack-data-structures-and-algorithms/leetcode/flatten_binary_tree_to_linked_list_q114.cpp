#include <utility>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 利用先序遍历的模式，先处理 node->left & node->right 再拼接；递归函数返回 new_head & new_tail
// 需要注意的是，node->left必须要置NULL
class Solution {
public:
    void flatten(TreeNode* root) {
        do_flatten(root);
    }

    // <new_head, new_tail>
    pair<TreeNode*, TreeNode*> do_flatten(TreeNode* node) {
        if (!node) {
            return {nullptr, nullptr};
        }

        auto lc = do_flatten(node->left);
        auto rc = do_flatten(node->right);

        TreeNode* tail = node;

        if (lc.first) {
            tail->left = nullptr;
            tail->right = lc.first;
            tail = lc.second;
        }

        if (rc.first) {
            tail->left = nullptr;
            tail->right = rc.first;
            tail = rc.second;
        }

        return {node, tail};
    }
};
