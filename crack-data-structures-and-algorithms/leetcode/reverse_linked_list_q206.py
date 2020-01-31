# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def reverse_list(head):
    if not head:
        return None
    prev = None
    cur = head
    while cur:
        nxt = cur.next
        cur.next = prev
        prev = cur
        cur = nxt
    return prev


def reverse_list_ex(head):
    if not head:
        return None

    dummy = ListNode(0)
    dummy.next = head
    pp = dummy
    p = head
    while p.next:
        pn = p.next
        p.next = pn.next
        pn.next = pp.next
        pp.next = pn

    return dummy.next
