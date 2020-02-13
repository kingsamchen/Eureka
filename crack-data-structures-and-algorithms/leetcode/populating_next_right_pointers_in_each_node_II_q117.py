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
# 递归处理，因为题目已经说过递归调用栈不算extra space
# 对于每一个节点，首先找到这个节点的不为空的，最靠右的孩子节点，作为下一层连接操作的左端
# 然后在这一层往右扫描，找到第一个有孩子结点的结点，他的最靠左的孩子节点作为连接端的右端
# 如果上一个操作能找到左右两端，则进行连接
# 然后递归处理当前节点的子结点情况。注意：一定要先处理右孩子，再处理左孩子，因为前面水平扫描是自左向右。
class Solution(object):
    def connect(self, root):
        """
        :type root: Node
        :rtype: Node
        """
        if not root:
            return root

        # connect children, even root.right == None is well taken.
        if root.left:
            root.left.next = root.right

        # locate the right part node of left children.
        lc_right = root.right if root.right else root.left

        # because a node may have no child, therefore
        # scan from left to right, stop at the first node who has at least one child.
        pn = root.next
        while pn and not (pn.left or pn.right):
            pn = pn.next

        # connect if may.
        if pn and lc_right:
            next_left = pn.left if pn.left else pn.right
            lc_right.next = next_left

        # recursively.
        # NOTE: must handle right part ahead of left part.
        # because our horizontal scan is from left to right.
        self.connect(root.right)
        self.connect(root.left)

        return root
