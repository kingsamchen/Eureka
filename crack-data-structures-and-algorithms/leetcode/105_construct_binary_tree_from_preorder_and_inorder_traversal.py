# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None

class Solution(object):
    def buildTree(self, preorder, inorder):
        """
        :type preorder: List[int]
        :type inorder: List[int]
        :rtype: TreeNode
        """
        if len(preorder) == 0:
            return None

        return build(preorder, 0, len(preorder), inorder, 0, len(inorder))


def build(preorder, pl, pr, inorder, il, ir):
    if pr - pl < 1 or ir - il < 1:
        return None

    root = TreeNode(preorder[pl])

    # locate root in inorder
    # left part is for left sub-tree
    # right part is for right sub-tree
    rii = find_in_range(inorder, il, ir, root.val)

    lc_cnt = rii - il
    lc_pl = pl + 1
    lc_pr = lc_pl + lc_cnt
    lc_il = il
    lc_ir = rii
    lc = build(preorder, lc_pl, lc_pr, inorder, lc_il, lc_ir)

    rc_cnt = pr - lc_pr
    rc_pl = lc_pr
    rc_pr = pr
    rc_il = rii + 1
    rc_ir = ir
    rc = build(preorder, rc_pl, rc_pr, inorder, rc_il, rc_ir)

    root.left = lc
    root.right = rc

    return root


def find_in_range(lst, l, r, val):
    for i in range(l, r):
        if lst[i] == val:
            return i
