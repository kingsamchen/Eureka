# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 维护列表部分[head, p] 为有序，每次检查pn
# 如果pn.val < p.val，确实需要调整插入，则利用pp作为probe，从dummy开始扫描
# 确定 pp.next 为 pn 要插入的位置
class Solution(object):
    def insertionSortList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        if not head or not head.next:
            return head

        dummy = ListNode(0)
        dummy.next = head

        p = head
        while p.next:
            pn = p.next
            if pn.val < p.val:
                pp = dummy
                while pn.val >= pp.next.val:
                    pp = pp.next
                p.next = pn.next
                pn.next = pp.next
                pp.next = pn
            else:
                # prune. since head ... p is already sorted
                p = p.next

        return dummy.next
