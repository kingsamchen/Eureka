# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def reverse_between(head, m, n):
    """
    :type head: ListNode
    :type m: int
    :type n: int
    :rtype: ListNode
    1 ≤ m ≤ n ≤ length of list.
    """
    if not head:
        return None

    # pp: pointer to prev
    # p: pointer to current
    # pn: pointer to next
    #       +-                -+
    # n1 -> |  n2 -> n3 -> n4  | -> n5
    # |     +- |     |        -+
    # pp       p     pn
    #
    #       +-                -+
    # n1 -> |  n3 -> n2 -> n4  | -> n5
    # |     +- |     |        -+
    # pp       pn    p
    #
    #       +-                -+
    # n1 -> |  n3 -> n2 -> n4  | -> n5
    # |     +-       |     |  -+
    # pp             p     pn
    #
    #       +-                -+
    # n1 -> |  n4 -> n3 -> n2  | -> n5
    # |     +- |           |  -+
    # pp       pn          p
    dummy = ListNode(0)
    dummy.next = head
    pp = dummy

    # range starts from 1
    pos = 1
    while pos < m:
        pp = pp.next
        pos += 1

    p = pp.next

    while pos < n:
        pn = p.next
        p.next = pn.next
        pn.next = pp.next
        pp.next = pn
        pos += 1

    return dummy.next
