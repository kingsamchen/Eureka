# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思想
# 用 extract-then-insert 调整一个pair，然后前进
class Solution(object):
    def swapPairs(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        if not head or not head.next:
            return head

        dummy = ListNode(0)
        dummy.next = head
        pp = dummy
        p = head
        # even length, p would finally point to nil
        # odd length, p would finally point to the last node.
        while p and p.next:
            pn = p.next
            p.next = pn.next
            pn.next = pp.next
            pp.next = pn
            # calibrate pp & p
            pp = p
            p = p.next

        return dummy.next
