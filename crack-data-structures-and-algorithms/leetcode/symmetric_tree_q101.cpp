struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 这题不依赖前中后序遍历，而是需要利用镜像一样的做法往下遍历子树，即
// left->left && right->right 应该对称，同时 left->right && right->left 应该对称
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) {
            return true;
        }
        return check_symmetric(root->left, root->right);
    }

    bool check_symmetric(TreeNode* left, TreeNode* right) {
        if (!left && !right) {
            return true;
        }

        if ((left && !right) || (!left && right) || left->val != right->val) {
            return false;
        }

        return check_symmetric(left->left, right->right) &&
               check_symmetric(left->right, right->left);
    }
};
