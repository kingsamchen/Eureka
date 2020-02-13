# Definition for a binary tree node.
# class TreeNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.left = None
#         self.right = None

# 核心思路
# 因为是BST，所以可以利用树的有序性质
# 1）如果p,q两个结点都在当前节点的左子树，即 root.val > max(p.val, q.val)，LCA在左子树里
# 2）如果两个结点都在当前节点右子树，即 root.val < min(p.val, q.val)，那么 LCA在右子树里
# 3）如果一左一右，那么当前节点就是LCA
class Solution(object):
    def lowestCommonAncestor(self, root, p, q):
        """
        :type root: TreeNode
        :type p: TreeNode
        :type q: TreeNode
        :rtype: TreeNode
        """
        return visit(root, p, q)


def visit(root, p, q):
    if not root:
        return None

    if root.val > max(p.val, q.val):
        return visit(root.left, p, q)

    if root.val < min(p.val, q.val):
        return visit(root.right, p, q)

    return root
