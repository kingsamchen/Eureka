# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心点：
# 如果结点i和节点j**之间**存在和为0的连续节点，即 acc(list[i:j]) == 0，那么acc[:i+1] == acc[:j]
# 举例：
# 结点值  1, 2, 3, -5, 4
# 累计和  1, 3, 6,  1, 5
# 连续结点 2, 3, -5 和为0，所以第一个节点和第四个结点的累积和一样，都是1
# 因此只需要用一个字典来不断更新每个累计和对应的最新的结点指针，最后重新遍历链表跳过中间部分即可。
class Solution(object):
    def removeZeroSumSublists(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        sum_table = {}

        dummy = ListNode(0)
        dummy.next = head

        acc = 0
        # tracking from dummy node, in case the entire list sums up to 0
        p = dummy
        while p:
            sum += p.val
            # later sum-node tracking would overwrite the previous, its fine.
            sum_table[acc] = p
            p = p.next

        acc = 0
        p = dummy
        while p:
            acc += p.val
            # Mismatch implies consecutive zero sum among p ... sum_table[acc], right part
            # is included.
            if sum_table[acc] != p:
                p.next = sum_table[acc].next
            p = p.next

        return dummy.next
