# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def swap_pairs(head):
    """
    :type head: ListNode
    :rtype: ListNode
    """
    if not head:
        return None

    dummy = ListNode(0)
    dummy.next = head
    pp = dummy
    p = head
    while p and p.next:
        pn = p.next
        p.next = pn.next
        pn.next = p
        pp.next = pn

        pp = p
        p = p.next

    return dummy.next
