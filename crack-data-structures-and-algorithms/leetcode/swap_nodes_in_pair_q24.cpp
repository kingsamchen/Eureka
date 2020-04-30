struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思想
// 用 extract-then-insert 调整一个pair；调整后 p 指向的结点已经在后面，所以 p.next 就是下一个要调整的 pair 的第一个节点
// 终止条件是 p && p.next 有效
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if (!head) {
            return nullptr;
        }

        ListNode dummy(0);
        dummy.next = head;

        auto pp = &dummy;
        auto p = head;
        while (p && p->next) {
            auto pn = p->next;
            p->next = pn->next;
            pn->next = pp->next;
            pp->next = pn;

            pp = p;
            p = p->next;
        }

        return dummy.next;
    }
};
