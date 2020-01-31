# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None


def sorted_array_2_bst(nums):
    """
    :type nums: List[int]
    :rtype: TreeNode
    """
    if len(nums) == 0:
        return None

    return gen_bst(nums, 0, len(nums))


def gen_bst(nums, l, r):
    cnt = r - l
    if cnt == 0:
        return None
    if cnt == 1:
        return TreeNode(nums[l])

    lc = cnt // 2
    rc = cnt - lc - 1
    mid = l + lc
    parent = TreeNode(nums[mid])
    parent.left = gen_bst(nums, l, mid)
    parent.right = gen_bst(nums, mid+1, r)

    return parent
