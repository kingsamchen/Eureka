# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 首先遍历得到链表长度，记为 N
# 对于每一轮，part的长度为 part_cnt = ceil(N / k) = N + k - 1 / k
# 此时的p位于part的头结点，然后往前移动 part_cnt - 1 个结点，位于part的最后一个结点上
# 将这个part断开，并转入下一个part，同时更新 N 和 k
# 注意：按照题目要求，如果 part_cnt == 0，则存入空指针
class Solution(object):
    def splitListToParts(self, root, k):
        """
        :type root: ListNode
        :type k: int
        :rtype: List[ListNode]
        """
        cnt = 0
        p = root
        while p:
            cnt += 1
            p = p.next

        p = root
        parts = []
        for i in range(k, 0, -1):
            # get count for this part
            part_cnt = (cnt + i - 1) // i
            if part_cnt == 0:
                parts.append(None)
            else:
                parts.append(p)
                # move forward part_cnt - 1 nodes
                # stop at the last node of the part
                for _ in range(1, part_cnt):
                    p = p.next
                pn = p.next
                p.next = None
                p = pn
            cnt -= part_cnt

        return parts
