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

    # Head node is also possible, thus dummy node is required.
    dummy = ListNode(0)
    dummy.next = head
    pp = dummy

    probe = p = head

    # locate the probe node
    # n -> distance between probe and p
    for _ in range(n):
        probe = probe.next

    while probe:
        probe = probe.next
        pp = p
        p = p.next

    pp.next = p.next

    return dummy.next
