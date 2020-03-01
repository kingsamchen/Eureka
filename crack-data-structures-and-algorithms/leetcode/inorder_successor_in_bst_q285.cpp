#include <vector>

using namespace std;

/**
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 本质框架还是迭代中序遍历
// 首先通过根节点遍历找到目标结点p，并把沿途的结点都入栈
// 如果结点p有右孩子，则它的后继一定在他右孩子的最左子节点里
// 否则，逐个弹出栈里的结点，检查是否有值大于p的，如果有就是后继结点
// 如果没有找到那么目标节点p无后继
class Solution {
public:
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        vector<TreeNode*> pending;
        auto node = root;
        while (node != p) {
            pending.push_back(node);
            if (node->val < p->val) {
                node = node->right;
            } else {
                node = node->left;
            }
        }

        if (node->right) {
            node = node->right;
            while (node->left) {
                node = node->left;
            }
            return node;
        }

        while (!pending.empty()) {
            node = pending.back();
            pending.pop_back();
            if (node->val > p->val) {
                return node;
            }
        }

        return nullptr;
    }
};
