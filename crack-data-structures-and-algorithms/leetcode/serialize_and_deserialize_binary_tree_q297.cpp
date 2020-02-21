#include <deque>
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
// 采用层序遍历的方式来做序列化/反序列化
// 注意：如果某个结点p的左右孩子都是nil，则两个nil孩子结点用$标记
// 反序列化的时候先做token切分
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        string data;
        deque<TreeNode*> pending{root};
        while (!pending.empty()) {
            auto node = pending.front();
            pending.pop_front();
            if (node) {
                data.append(to_string(node->val));
                pending.push_back(node->left);
                pending.push_back(node->right);
            } else {
                // Mark as nil
                data.append("$");
            }
            data.append(" ");
        }

        return data;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        // Token split
        vector<string> values;
        istringstream in(data);
        string token;
        while (getline(in, token, ' ')) {
            values.push_back(token);
        }

        // Empty tree case
        if (values[0] == "$") {
            return nullptr;
        }

        auto root = new TreeNode(stoi(values[0]));
        deque<TreeNode*> pending {root};
        auto it = values.begin() + 1;
        while (!pending.empty()) {
            auto node = pending.front();
            pending.pop_front();

            node->left = (*it == "$") ? nullptr : new TreeNode(stoi(*it));
            ++it;
            node->right = (*it == "$") ? nullptr : new TreeNode(stoi(*it));
            ++it;

            if (node->left) {
                pending.push_back(node->left);
            }

            if (node->right) {
                pending.push_back(node->right);
            }
        }

        return root;
    }
};
