# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None

# 核心思路：
# 对于 level order traversal，直接使用BFS遍历即可
# 维护一个数组存储下一层需要遍历的结点
# 同时遍历当层结点时按顺序存储，最后根据层direction来决定正向还是反向
class Solution(object):
    def zigzagLevelOrder(self, root):
        """
        :type root: TreeNode
        :rtype: List[List[int]]
        """
        if not root:
            return []

        results = []

        pending = [root]
        direction = 1
        while len(pending) > 0:
            vals = []
            sub = []
            for node in pending:
                vals.append(node.val)
                if node.left:
                    sub.append(node.left)
                if node.right:
                    sub.append(node.right)

            results.append(vals[::direction])

            direction = -direction
            pending = sub

        return results
