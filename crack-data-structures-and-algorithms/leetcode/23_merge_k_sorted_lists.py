# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


# For lists [l1, l2, l3, ..., ln-1, ln], we merge l1 & ln and save result at l1;
# merge l2 & ln-1 and save result at l2, etc.
# We repeat merge until count of lists need to merge is no more than 1.
# NOTE: `r` always points to the right-bound of lists in next round.
# l1, l2, l2, ..., ln-1, ln
#   l1, l2, ..., ln
#       ...
#      l1, l2
#        l1
# Just like how merge process of merge-sort works.
# Complexity: O(nlogk)
class Solution(object):
    def mergeKLists(self, lists):
        """
        :type lists: List[ListNode]
        :rtype: ListNode
        """
        cnt = len(lists)
        if cnt == 0:
            return None
        if cnt == 1:
            return lists[0]

        l, r = 0, len(lists) - 1
        while cnt > 1:
            while l < r:
                lists[l] = merge_two_lists(lists[l], lists[r])
                l += 1
                r -= 1

            # Reset
            l = 0
            cnt = r - l + 1

        return lists[0]


def merge_two_lists(l1, l2):
    dummy = ListNode(0)
    p = dummy
    while l1 and l2:
        if l1.val < l2.val:
            p.next = l1
            l1 = l1.next
        else:
            p.next = l2
            l2 = l2.next

        p = p.next

    if l1:
        p.next = l1

    if l2:
        p.next = l2

    return dummy.next
