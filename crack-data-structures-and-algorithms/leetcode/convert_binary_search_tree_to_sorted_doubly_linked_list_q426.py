"""
# Definition for a Node.
class Node(object):
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right
"""
class Solution(object):
    def treeToDoublyList(self, root):
        """
        :type root: Node
        :rtype: Node
        """
        if not root:
            return None

        # Since root has at least one node, head & tail cannot be None.
        head, tail = flatten(root)
        head.left = tail
        tail.right = head
        return head

def flatten(root):
    if not root:
        return None, None

    lhead, ltail = flatten(root.left)
    rhead, rtail = flatten(root.right)

    root.left = ltail
    root.right = rhead

    if ltail:
        ltail.right = root
    if rhead:
        rhead.left = root

    # Determine the head and the tail.
    head = lhead if lhead else root
    tail = rtail if rtail else root

    return head, tail
