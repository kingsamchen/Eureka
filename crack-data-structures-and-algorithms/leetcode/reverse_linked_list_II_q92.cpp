
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思路
// 使用 extract-then-insert 处理框架
// 一开始令 p 指向第 m 个结点，然后每次操作，将 pn (pn = p.next) 结点抽出，插入 pp 结点后面
// 不断循环一直到处理完第 n 个结点
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int m, int n) {
        if (!head) {
            return nullptr;
        }

        ListNode dummy(0);
        dummy.next = head;
        ListNode* pp = &dummy;

        int pos = 1;
        // Move forward m - pos steps to arrive at start
        for (; pos < m; ++pos) {
            pp = pp->next;
        }

        auto p = pp->next;

        // extract and insert
        for (; pos < n; ++pos) {
            auto pn = p->next;
            p->next = pn->next;
            pn->next = pp->next;
            pp->next = pn;
        }

        return dummy.next;
    }
};
