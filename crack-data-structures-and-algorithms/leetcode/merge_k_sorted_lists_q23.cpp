#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思路
// 首先有一个可以合并两个给定有序链表的函数，merge_two_lists()
// 然后采用双指针法，l & r两个游标往中间靠拢，合并lists[l] & lists[r]并将结果存储 lists[l]
// 当 l == r时，这一轮结束；保持 r 不变，下一轮l继续从0开始
// 一直到 r == 0，则最终合并后的链表就存在 lists[0] 中
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) {
            return nullptr;
        }

        int r = lists.size() - 1;
        // when r == 0, we merged all lists down to one.
        while (r > 0) {
            for (auto l = 0; l < r; ++l, --r) {
                auto l1 = lists[l];
                auto l2 = lists[r];
                auto new_list = merge_two_lists(l1, l2);
                lists[l] = new_list;
            }
        }

        return lists[0];
    }

    ListNode* merge_two_lists(ListNode* l1, ListNode* l2) {
        auto dummy = ListNode(0);
        auto p = &dummy;
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

        return dummy.next;
    }
};
