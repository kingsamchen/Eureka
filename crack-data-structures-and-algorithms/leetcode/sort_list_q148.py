# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


class Solution(object):
    def sort_list(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        if not head:
            return None

        return sort_list(head)


def sort_list(head):
    if not head or not head.next:
        return head

    slow = fast = head
    while fast.next and fast.next.next:
        fast = fast.next.next
        slow = slow.next

    l1 = head
    l2 = slow.next
    slow.next = None

    l1 = sort_list(l1)
    l2 = sort_list(l2)

    return merge_lists(l1, l2)


def merge_lists(l1, l2):
    # No need to check l1 & l2 up front
    dummy = ListNode(0)
    dummy.next = l1
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
