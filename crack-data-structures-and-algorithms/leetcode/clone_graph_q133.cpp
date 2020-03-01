#include <deque>
#include <unordered_map>

using namespace std;

// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;

    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }

    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }

    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};

// 核心思路
// 使用一个 hash-table 维护源结点 node 到其克隆结点的映射
// 利用 BFS 每次从队列弹出一个原结点src，然后将src的neib结点逐个克隆到clone的neib去
// 如果有个neib不在映射表里，说明之前没有处理过，需要加入pending队列进行后续遍历处理
class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) {
            return nullptr;
        }

        unordered_map<Node*, Node*> table;
        table[node] = new Node(node->val);

        deque<Node*> pending {node};
        while (!pending.empty()) {
            auto cur = pending.front();
            pending.pop_front();
            for (auto neib : cur->neighbors) {
                // neib node hasn't processed yet.
                // add it into pending list.
                if (table.count(neib) == 0) {
                    table[neib] = new Node(neib->val);
                    pending.push_back(neib);
                }

                table[cur]->neighbors.push_back(table[neib]);
            }
        }

        return table[node];
    }
};
