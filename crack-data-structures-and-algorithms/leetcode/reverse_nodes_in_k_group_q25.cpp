struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思路
// 每个group的节点用extract-and-insert技法进行翻转，每次反转后 p.next 刚好是下一个要翻转的结点
// 整体结束后更新 pp & p
// 因为不满足一个group的结点要保持原样，所以每次对一个group操作前需要检查当前结点开始是否能满足一个group（has_k_nodes）
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (!head) {
            return nullptr;
        }

        ListNode dummy(0);
        dummy.next = head;

        auto pp = &dummy;
        auto p = head;

        while (has_k_nodes(p, k)) {
            for (int j = 0; j < k - 1; ++j) {
                auto pn = p->next;
                p->next = pn->next;
                pn->next = pp->next;
                pp->next = pn;
            }

            pp = p;
            p = p->next;
        }

        return dummy.next;
    }

    bool has_k_nodes(ListNode* p, int k) {
        for (int i = 0; i < k; ++i) {
            if (!p) {
                return false;
            }
            p = p->next;
        }
        return true;
    }
};
