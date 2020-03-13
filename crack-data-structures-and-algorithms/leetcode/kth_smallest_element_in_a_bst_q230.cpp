
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 采用中序遍历方式来计算已经遍历过的结点；我们的目标是遍历的第k个结点
// 每次遍历一个结点，从k减去1，如果当前节点遍历结束后 k == 0，则当前节点就是目标节点
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        return find_kth(root, k);
    }

    int find_kth(TreeNode* root, int& k) {
        if (!root) {
            return -1;
        }

        auto val = find_kth(root->left, k);
        if (k == 0) {
            return val;
        }

        --k;
        if (k == 0) {
            return root->val;
        }

        return find_kth(root->right, k);
    }
};
