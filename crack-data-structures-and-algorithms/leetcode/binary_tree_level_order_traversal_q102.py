# Definition for a binary tree node.
# class TreeNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.left = None
#         self.right = None

# 核心思路
# 层次遍历用BFS，用队列维护后面需要遍历的结点
# 考虑到需要按层保存，所以其实可以不用队列，新旧swap即可
class Solution(object):
    def levelOrder(self, root):
        """
        :type root: TreeNode
        :rtype: List[List[int]]
        """
        if not root:
            return []

        result = []

        pending = [root]
        while len(pending) > 0:
            next_layer = []
            for node in pending:
                if node.left:
                    next_layer.append(node.left)
                if node.right:
                    next_layer.append(node.right)

            result.append([n.val for n in pending])
            pending = next_layer

        return result
