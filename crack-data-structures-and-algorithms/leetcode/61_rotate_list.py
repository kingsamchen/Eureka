# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def rotate_right(head, k):
    """
    :type head: ListNode
    :type k: int
    :rtype: ListNode
    """
    if not head:
        return None

    cnt = 0
    p = head
    while p:
        p = p.next
        cnt += 1

    dist = k % cnt
    if dist == 0:
        return head

    ps = probe = head
    i = 0
    while i < dist:
        probe = probe.next
        i += 1

    prev = None
    while probe:
        probe = probe.next
        prev = ps
        ps = ps.next

    pe = ps
    while pe.next:
        pe = pe.next

    pe.next = head
    prev.next = None

    return ps


def rotate_right2(head, k):
    if not head:
        return None

    cnt = 1
    p = head
    while p.next:
        cnt += 1
        p = p.next

    p.next = head

    lcnt = cnt - k % cnt

    # p currently points to the tail node.
    # thus move lcnt steps to locate the new last node.
    i = 0
    while i < lcnt:
        p = p.next
        i += 1

    nhead = p.next
    p.next = None

    return nhead
