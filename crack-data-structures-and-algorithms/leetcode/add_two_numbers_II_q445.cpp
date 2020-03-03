#include <utility>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// 核心思路
// 先求出两个链表的长度，并且保证 |l1| >= |l2|
// 维护两个结点指针：p指向结果链表的当前结点；pnc指向结果链表最近一个不为9的结点，保证自身加一后不进位
// 先对l1遍历 |l1| - |l2| 个节点，保证l1 & l2第二阶段可以齐头并进；迭代中记得更新pnc
// 再一起遍历两个链表，此时l1&l2长度一致。
// 1）如果结点和>9，则说明要发生进位，此时 pnc->val++，并且pnc->next开始的节点都要置为0，因为这些节点值都是9
// 进位会从低位传递到高位。
// 此时进位处理完毕，创建新节点并移动p（值为 sum%10），并且因为这个结点只肯定小于9，所以pnc也要更新
// 2）如果节点和 <=9，则正常创建新节点并移动p；如果 val != 9 则也要更新pnc
// 最后，如果两个结点最高位发生了进位，那么 dummy->val 会因为进位变成1
// 最终结果需要判断是 dummy 还是 dummy->next
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        auto len1 = list_length(l1);
        auto len2 = list_length(l2);

        if (len1 < len2) {
            swap(l1, l2);
            swap(len1, len2);
        }

        auto dummy = new ListNode(0);
        auto p = dummy;
        auto pnc = dummy;

        auto dist = len1 - len2;
        for (auto i = 0; i < dist; ++i) {
            p->next = new ListNode(l1->val);
            p = p->next;
            if (p->val != 9) {
                pnc = p;
            }

            l1 = l1->next;
        }

        while (l1) {
            auto sum = l1->val + l2->val;
            if (sum > 9) {
                pnc->val++;
                while (pnc->next) {
                    pnc = pnc->next;
                    pnc->val = 0;
                }
                p->next = new ListNode(sum % 10);
                p = p->next;
                pnc = p;
            } else {
                p->next = new ListNode(sum);
                p = p->next;
                if (p->val != 9) {
                    pnc = p;
                }
            }

            l1 = l1->next;
            l2 = l2->next;
        }

        return dummy->val == 0 ? dummy->next : dummy;
    }

    int list_length(ListNode* node) {
        int count = 0;
        while (node) {
            ++count;
            node = node->next;
        }

        return count;
    }
};
