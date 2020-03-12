
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思路
// 同时扫描两条链表，根据判断结果p每次选择后继结点
// 如果某条链表已经被扫描完毕了另外一条链表还有剩，就直接把剩下的拼接在一起
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        auto dummy = new ListNode(0);
        auto p = dummy;
        while (l1 && l2) {
            if (l1->val < l2->val) {
                p->next = l1;
                l1 = l1->next;
            } else {
                p->next = l2;
                l2 = l2->next;
            }

            p = p->next;
        }

        if (l1) {
            p->next = l1;
        }

        if (l2) {
            p->next = l2;
        }

        return dummy->next;
    }
};
