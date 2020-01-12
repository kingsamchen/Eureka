# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


class Solution(object):
    def addTwoNumbers(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        dummy = ListNode(0)
        p = dummy
        cf = 0
        while l1 or l2:
            v1 = l1.val if l1 else 0
            v2 = l2.val if l2 else 0
            val = v1 + v2 + cf

            cf = val // 10
            val = val % 10

            p.next = ListNode(val)
            p = p.next

            if l1:
                l1 = l1.next

            if l2:
                l2 = l2.next

        if cf != 0:
            p.next = ListNode(1)

        return dummy.next
