# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


def deleteDuplicates(head):
    """
    :type head: ListNode
    :rtype: ListNode
    """
    # Use dummy node to resolve head node issue: duplicates happen at head
    # Now
    # dummy -> node1 -> node2 -> node3 ... -> noden
    #            ^
    #            |
    #           head
    # at first, we make dummy.next = head and prev = dummy
    # whenever no adjacent duplicates, move forward prev
    # otherwise, prev.next = new_node_pos
    dummy = ListNode(0)
    dummy.next = head
    prev = dummy
    p = head
    while p and p.next:
        if p.val != p.next.val:
            prev = p
            p = p.next
        else:
            t = p.next.next
            while t and p.val == t.val:
                t = t.next
            prev.next = t
            p = t
    return dummy.next