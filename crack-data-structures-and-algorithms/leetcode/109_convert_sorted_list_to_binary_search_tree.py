# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None


def sorted_list_2_bst(head):
    """
    :type head: ListNode
    :rtype: TreeNode
    """
    if not head:
        return None

    cnt = 0
    p = head
    while p:
        cnt += 1
        p = p.next

    return gen_bst(head, cnt)


def gen_bst(head, cnt):
    if cnt == 0:
        return None

    if cnt == 1:
        return TreeNode(head.val)

    left = cnt // 2
    right = cnt - left - 1

    i = 0
    pm = head
    while i < left:
        pm = pm.next
        i += 1

    right_head = pm.next

    parent = TreeNode(pm.val)
    parent.left = gen_bst(head, left)
    parent.right = gen_bst(right_head, right)

    return parent
