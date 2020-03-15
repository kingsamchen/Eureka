#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 核心思路
// 因为是BST，所以只需要保存 preorder 遍历结果即可；inorder 遍历的结果可以直接对 preorder 的排序得到
// 所以反序列化的时候先拿到 preorder 的遍历结果，然后拷贝一份对副本排序得到 inorder 遍历结果
// 接下来就是直接建树
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        string data;
        preorder(root, data);
        return data;
    }

    void preorder(TreeNode* root, string& data) {
        if (!root) {
            return;
        }

        data += to_string(root->val);
        data.append(1, ',');

        preorder(root->left, data);
        preorder(root->right, data);
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        istringstream input(data);
        string val;
        vector<int> pre;
        while (getline(input, val, ',')) {
            pre.push_back(stoi(val));
        }

        vector<int> in = pre;
        sort(in.begin(), in.end());

        return build_tree(pre.begin(), pre.end(), in.begin(), in.end());
    }

    template<typename ITER>
    TreeNode* build_tree(ITER pre_begin, ITER pre_end, ITER in_begin, ITER in_end) {
        if (pre_begin == pre_end) {
            return nullptr;
        }

        auto root = new TreeNode(*pre_begin);

        auto inorder_it = find(in_begin, in_end, root->val);
        int left_cnt = distance(in_begin, inorder_it);
        auto pre_right_begin = pre_begin + 1 + left_cnt;

        auto lc = build_tree(pre_begin + 1, pre_right_begin, in_begin, inorder_it);
        auto rc = build_tree(pre_right_begin, pre_end, inorder_it + 1, in_end);

        root->left = lc;
        root->right = rc;

        return root;
    }
};
