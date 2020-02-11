"""
# Definition for a Node.
class Node(object):
    def __init__(self, val=0, left=None, right=None, next=None):
        self.val = val
        self.left = left
        self.right = right
        self.next = next
"""

# 核心思路
# 为了连接第N+1层的结点的sibling，我们需要在第N层操作（因为需要访问左右子结点）
# 利用start来逐层向下，并且在每一层用p遍历每一个节点；这一层的sibling已经在遍历上一层时连接完毕
# 所以遍历这一层的时候需要连接下一层的siblings
class Solution(object):
    def connect(self, root):
        """
        :type root: Node
        :rtype: Node
        """
        if not root:
            return root

        start = root
        # When populating layer N+1, we are actually visiting layer N.
        # we use start to go down into each layer, and use p to visit that layer.
        while start.left:
            p = start
            while p:
                p.left.next = p.right
                if p.next:
                    p.right.next = p.next.left
                p = p.next
            start = start.left

        return root