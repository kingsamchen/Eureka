# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def remove_nth_from_end(head, n):
    """
    :type head: ListNode
    :type n: int
    :rtype: ListNode
    """
    if not head:
        return None

    dummy = ListNode(0)
    dummy.next = head
    prev = dummy
    probe = cur = head
    i = 0
    while i < n:
        probe = probe.next
        i += 1

    while probe:
        probe = probe.next
        cur = cur.next
        prev = prev.next

    prev.next = cur.next

    return dummy.next
