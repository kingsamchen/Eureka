#include <algorithm>
#include <utility>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 递归遍历左右子树
// 如果一侧已经不平衡，直接不断向上返回结果
// 如果两边高度差已经大于1，则直接返回不平衡结果
// 否则当前节点的高度为：左右子树的max加上1（自身），向上回溯
class Solution {
public:
    bool isBalanced(TreeNode* root) {
        auto ans = calc_height(root);
        return ans.first;
    }

    pair<bool, int> calc_height(TreeNode* node) {
        if (!node) {
            return {true, 0};
        }

        auto left = calc_height(node->left);
        if (!left.first) {
            return left;
        }

        auto right = calc_height(node->right);
        if (!right.first) {
            return right;
        }

        auto diff = abs(left.second - right.second);
        return {diff <= 1, max(left.second, right.second) + 1};
    }
};
