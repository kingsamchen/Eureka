#include <algorithm>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 当前结点深度=max(左孩子深度, 右孩子深度)
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) {
            return 0;
        }

        auto ldep = maxDepth(root->left);
        auto rdep = maxDepth(root->right);
        return max(ldep, rdep) + 1;
    }
};