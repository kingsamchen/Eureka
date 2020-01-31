# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def reorder_list(head):
    if not (head and head.next and head.next.next):
        return head

    slow = fast = head
    while fast.next and fast.next.next:
        fast = fast.next.next
        slow = slow.next

    # reverse right part
    pr = slow.next
    # cutoff
    slow.next = None
    pp = None
    while pr:
        pn = pr.next
        pr.next = pp
        pp = pr
        pr = pn

    # pp now points to the 1st node of the reversed right part
    p = head
    pr = pp
    while pr:
        pn = pr.next
        pr.next = p.next
        p.next = pr
        pr = pn
        p = p.next.next

    return head


def reorder_list_mine_original(head):
    if not head:
        return None

    slow = fast = head
    # prev of slow
    pp = None
    # pr: right part starts
    pr = None
    while fast and fast.next:
        fast = fast.next.next
        # move slow one step and reverse crossed nodes
        pn = slow.next
        pr = pn.next
        slow.next = pp
        pp = slow
        slow = pn

    if not pr:
        return head

    p = slow if fast else pp
    while pr:
        pn = pr.next
        pr.next = p.next
        p.next = pr
        pr = pn
        p = p.next.next

    p = slow
    pp = None
    while p:
        pn = p.next
        p.next = pp
        pp = p
        p = pn

    return head
