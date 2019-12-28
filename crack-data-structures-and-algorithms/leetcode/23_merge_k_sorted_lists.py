# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def merge_lists(lists):
    """
    :type lists: List[ListNode]
    :rtype: ListNode
    """
    cnt = len(lists)
    l, r = 0, len(lists) - 1
    while cnt > 1:
        while l < r:
            lists[l] = merge_two_lists(lists[l], lists[r])
            l += 1
            r -= 1
        l = 0
        cnt = r - l + 1
    return lists[0]


# Returns head of merged list
def merge_two_lists(la, lb):
    dummy = ListNode(0)
    p = dummy
    while la and lb:
        if la.val < lb.val:
            p.next = la
            p = p.next
            la = la.next
        else:
            p.next = lb
            p = p.next
            lb = lb.next
    if la:
        p.next = la
    else:
        p.next = lb
    return dummy.next
