# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for a Node.
class Node:
    def __init__(self, x, next=None, random=None):
        self.val = int(x)
        self.next = next
        self.random = random


def copy_random_list(head):
    """
    :type head: Node
    :rtype: Node
    """
    if not head:
        return None

    # for old list
    node_idx_table = {}
    # for new list
    idx_node_list = []

    dummy = Node(0)
    pnew = dummy
    pold = head
    old_idx = 0
    while pold:
        node_idx_table[pold] = old_idx
        old_idx += 1
        pnew.next = Node(pold.val)
        pnew = pnew.next
        idx_node_list.append(pnew)
        pold = pold.next

    pold = head
    pnew = dummy.next
    while pnew:
        if pold.random:
            idx = node_idx_table[pold.random]
            pnew.random = idx_node_list[idx]
        pnew = pnew.next
        pold = pold.next

    return dummy.next
