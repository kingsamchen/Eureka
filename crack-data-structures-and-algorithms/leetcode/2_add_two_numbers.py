# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def add_two_numbers(l1, l2):
    """
    :type l1: ListNode
    :type l2: ListNode
    :rtype: ListNode
    """
    if not l1 or not l2:
        return None

    # 1 <- 2 <- 4           l1
    # 0 <- 3 <- 7           l2
    # prepend imaginary 0 to make two lists of equal length.
    # p always points to the current result digit.
    dummy = ListNode(0)
    p = dummy
    cf = 0
    while l1 or l2:
        val1 = l1.val if l1 else 0
        val2 = l2.val if l2 else 0
        val = cf + val1 + val2
        cf = val // 10
        if cf:
            val -= 10
        p.next = ListNode(val)
        p = p.next

        if l1:
            l1 = l1.next

        if l2:
            l2 = l2.next

        # prune: splice the remaining part directly.
        if not(l1 and l2) and cf == 0:
            if l1:
                p.next = l1
                break
            elif l2:
                p.next = l2
                break
    if cf != 0:
        p.next = ListNode(1)

    return dummy.next
