# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 这题是 add_two_numbers_II_q445.py 的特例版本，做法更简单
# 另，这题没说不能修改原链表，所以可以先reverse，变成低位在前
# 处理之后再 reverse 回去
class Solution(object):
    def plusOne(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        dummy = ListNode(0)
        dummy.next = head
        pnc = p = dummy
        while p.next:
            if p.val != 9:
                pnc = p
            p = p.next

        val = p.val + 1
        if val > 9:
            p.val = 0
            pnc.val += 1
            while pnc.next != p:
                pnc.next.val = 0
                pnc = pnc.next
        else:
            p.val = val

        return dummy.next if dummy.val == 0 else dummy
