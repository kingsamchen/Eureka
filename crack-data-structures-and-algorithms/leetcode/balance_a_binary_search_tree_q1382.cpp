#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 利用中序遍历得到有序节点集，然后以中点为根重建二叉树
class Solution {
public:
    TreeNode* balanceBST(TreeNode* root) {
        if (!root) {
            return nullptr;
        }

        vector<int> values;
        inorder_visit(root, values);
        return construct(values, 0, values.size() - 1);
    }

    TreeNode* construct(const vector<int>& values, int l, int r) {
        if (l > r) {
            return nullptr;
        }

        auto m = (l + r) / 2;
        auto root = new TreeNode(values[m]);
        auto lc = construct(values, l, m - 1);
        auto rc = construct(values, m + 1, r);
        root->left = lc;
        root->right = rc;
        return root;
    }

    void inorder_visit(TreeNode* root, vector<int>& values) {
        if (!root) {
            return;
        }

        inorder_visit(root->left, values);
        values.push_back(root->val);
        inorder_visit(root->right, values);
    }
};
