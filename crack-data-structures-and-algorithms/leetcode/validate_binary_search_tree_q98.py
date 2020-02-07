# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None

# 核心思路
# 对于一个结点p，有 p.left.val < p.val && p.val < p.right.val
# 同时p的左子树所有结点的值要小于p.val，即p.val是左子树结点值的upper-bound
# p的右子树所有结点的值要大于p.val，即p.val是右子树结点的lower-bound
class Solution(object):
    def isValidBST(self, root):
        """
        :type root: TreeNode
        :rtype: bool
        """
        # sentinel，用来过测试用例
        INT_MIN = -(1 << 61)
        INT_MAX = 1 << 61 - 1
        return validate(root, INT_MIN, INT_MAX)


def validate(root, lb, ub):
    if not root:
        return True

    if root.left:
        if root.left.val >= root.val or not (lb < root.left.val < ub):
            return False

    if root.right:
        if root.right.val <= root.val or not (lb < root.right.val < ub):
            return False

    return validate(root.left, lb, min(ub, root.val)) and validate(root.right, max(lb, root.val), ub)
