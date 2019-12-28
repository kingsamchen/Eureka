# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def mergeTwoLists(l1, l2):
    """
    :type l1: ListNode
    :type l2: ListNode
    :rtype: ListNode
    """
    dummy = ListNode(0)
    p = dummy
    while l1 and l2:
        if l1.val < l2.val:
            p.next = l1
            p = p.next
            l1 = l1.next
        else:
            p.next = l2
            p = p.next
            l2 = l2.next

    if l1:
        p.next = l1
    else:
        p.next = l2


    return dummy.next