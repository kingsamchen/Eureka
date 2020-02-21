#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 迭代版本的实现需要用栈辅助
// 遍历时只要当前结点有效，就将结点入栈，然后往左子节点移动
// 从栈出来的结点1）访问值 2）往右子节点移动
// 总结：先入栈，再往左；出栈后先访问再往右
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        vector<TreeNode*> bk;
        auto node = root;
        while (node || !bk.empty()) {
            while (node) {
                bk.push_back(node);
                node = node->left;
            }

            node = bk.back();
            bk.pop_back();
            result.push_back(node->val);
            node = node->right;
        }

        return result;
    }
};