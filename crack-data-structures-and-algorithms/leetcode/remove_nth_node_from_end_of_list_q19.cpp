
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思路
// 这里需要用到先后指针（但是为了变量命名方便，仍然使用 slow/fast）
// 1) 初始时 slow & fast 都指向 dummy
// 2) 为了移除倒数第 n 个结点，fast 指针提前先走 n 步
// 3) slow & fast 同时往前遍历，结束条件是 fast.next == null
//    循环结束后，fast 指向最后一个结点，slow.next 是要删除的结点（为了删除某个结点必须要指向它的prev）
// 4) 删除 slow 后继，也就是目标结点
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0);
        dummy.next = head;

        auto fast = &dummy;
        auto slow = &dummy;

        for (int i = 0; i < n; ++i) {
            fast = fast->next;
        }

        while (fast->next) {
            fast = fast->next;
            slow = slow->next;
        }

        auto pn = slow->next;
        slow->next = pn->next;

        return dummy.next;
    }
};
