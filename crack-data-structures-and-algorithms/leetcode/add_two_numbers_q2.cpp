
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思路
// 同时遍历两个链表，但是要遍历 max(|l1|, |l2|) 个节点，即以长的节点为准
// 这样长链表多出来的节点也会以正常逻辑被加到结果链表里
// 如果最后 carry=1，则说明最后需要一个进位结点
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        auto dummy = new ListNode(0);
        auto p = dummy;
        auto carry = 0;
        while (l1 || l2) {
            auto val1 = l1 ? l1->val : 0;
            auto val2 = l2 ? l2->val : 0;
            auto sum = val1 + val2 + carry;
            carry = sum / 10;
            sum %= 10;

            p->next = new ListNode(sum);
            p = p->next;

            if (l1) {
                l1 = l1->next;
            }

            if (l2) {
                l2 = l2->next;
            }
        }

        if (carry > 0) {
            p->next = new ListNode(1);
        }

        return dummy->next;
    }
};
