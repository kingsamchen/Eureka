# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def insertion_sort_list(head):
    if not(head and head.next):
        return head

    dummy = ListNode(0)
    dummy.next = head

    p = head
    while p.next:
        pn = p.next
        if pn.val < p.val:
            pp = dummy
            while pn.val > pp.next.val:
                pp = pp.next
            p.next = pn.next
            pn.next = pp.next
            pp.next = pn
        else:
            p = p.next

    return dummy.next
