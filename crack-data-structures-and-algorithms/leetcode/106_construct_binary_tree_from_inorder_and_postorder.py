# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None

class Solution(object):
    def buildTree(self, inorder, postorder):
        """
        :type inorder: List[int]
        :type postorder: List[int]
        :rtype: TreeNode
        """
        if len(inorder) == 0:
            return None
        return build(inorder, 0, len(inorder), postorder, 0, len(postorder))


def build(inorder, il, ir, postorder, pl, pr):
    if ir - il < 1 or pr - pl < 1:
        return None

    # last element in postorder is the root.
    # and parition inorder into two parts
    root = TreeNode(postorder[pr-1])
    rii = locate(inorder, il, ir, root.val)

    lc_cnt = rii - il
    lc = build(inorder, il, rii, postorder, pl, pl + lc_cnt)

    rc_cnt = ir - (rii + 1)
    rc = build(inorder, rii + 1, ir, postorder, pr - 1 - rc_cnt, pr - 1)

    root.left = lc
    root.right = rc

    return root

def locate(lst, l, r, val):
    for i in range(l, r):
        if lst[i] == val:
            return i
