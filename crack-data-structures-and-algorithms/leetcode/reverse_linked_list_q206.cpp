#include <utility>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思路
// pp, p, pn 经典三指针
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        decltype(head) pp = nullptr;
        auto p = head;
        while (p) {
            auto pn = p->next;
            p->next = pp;
            pp = p;
            p = pn;
        }

        return pp;
    }
};

// 递归版本
// 核心思路
// reverse()函数返回新的head, tail指针对
// 保持这个定义就可以
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        auto cp = reverse(head);
        return cp.first;
    }

    // new head & tail respectively
    pair<ListNode*, ListNode*> reverse(ListNode* head) {
        if (!head || !head->next) {
            return {head, head};
        }

        auto cp = reverse(head->next);
        cp.second->next = head;
        head->next = nullptr;
        return {cp.first, head};
    }
};
