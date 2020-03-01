
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// Sedgwick的算法删除思路
// 递归找到要删除的结点
// 如果结点没有孩子或者只有一个孩子，则返回另外一个孩子结点（没有孩子的时候返回的是null，无所谓）
// 如果有两个孩子，则首先找到后继节点，也就是当前节点右子树的最小节点
// 然后再得到当前节点右子树剔除掉刚才找到最小节点的新BST的结点，设置为后继节点的右孩子
// 然后再将要删除的节点的左孩子设置为后继节点的左孩子。
// 设置左右孩子顺序不能乱，一定是先右再左，否则会让后继节点产生环
// 如果要考虑内存不泄露，注意删除操作释放节点
class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) {
            return nullptr;
        }

        if (key < root->val) {
            root->left = deleteNode(root->left, key);
            return root;
        } else if (key > root->val) {
            root->right = deleteNode(root->right, key);
            return root;
        } else {
            if (!root->left) {
                return root->right;
            } else if (!root->right) {
                return root->left;
            } else {
                auto successor = MinNode(root->right);
                successor->right = DeleteMin(root->right);
                successor->left = root->left;
                return successor;
            }
        }
    }

    TreeNode* MinNode(TreeNode* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    TreeNode* DeleteMin(TreeNode* node) {
        if (!node->left) {
            return node->right;
        }
        node->left = DeleteMin(node->left);
        return node;
    }
};
