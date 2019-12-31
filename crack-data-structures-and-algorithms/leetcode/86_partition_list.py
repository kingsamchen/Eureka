# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def partition(head, x):
    """
    :type head: ListNode
    :type x: int
    :rtype: ListNode
    """
    if not head:
        return None

    dummy = ListNode(0)
    dummy.next = head
    pp = dummy
    p = dummy
    while p.next:
        pn = p.next
        if pn.val < x:
            if pp.next == pn:
                pp = pp.next
                p = p.next
            else:
                p.next = pn.next
                pn.next = pp.next
                pp.next = pn
                pp = pn
        else:
            p = p.next

    return dummy.next
