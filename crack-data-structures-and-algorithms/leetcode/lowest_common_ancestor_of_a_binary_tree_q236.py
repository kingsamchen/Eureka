# Definition for a binary tree node.
# class TreeNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.left = None
#         self.right = None

# 核心思路
# 因为没有BST的性质，所以这里只能常规遍历
# 遍历时候返回：确定p/q的数量 & LCA的结点
# 自底向上遍历，当某个结点的左、右子树已经找齐了，则把LCA结点返回；如果左右子树数量，加上当前节点（如果是pq之一）
# 等于2，则说明当前节点就是LCA
class Solution(object):
    def lowestCommonAncestor(self, root, p, q):
        """
        :type root: TreeNode
        :type p: TreeNode
        :type q: TreeNode
        :rtype: TreeNode
        """
        _, node = visit(root, p, q)
        return node


# return score, node(optional)
def visit(root, p, q):
    if not root:
        return 0, None

    lscore, lnode = visit(root.left, p, q)
    if lscore == 2:
        return lscore, lnode

    rscore, rnode = visit(root.right, p, q)
    if rscore == 2:
        return rscore, rnode

    score = 1 if root == p or root == q else 0
    score += lscore + rscore
    return score, root
