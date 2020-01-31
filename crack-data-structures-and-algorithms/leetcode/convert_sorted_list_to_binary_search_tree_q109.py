# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None


class Solution(object):
    def sortedListToBST(self, head):
        """
        :type head: ListNode
        :rtype: TreeNode
        """
        return build_bst(head, None)


# Range works like [begin, end)
def build_bst(begin, end):
    if begin == end:
        return None

    # begin cannot be None as here.
    if begin.next == end:
        return TreeNode(begin.val)

    # we use fast & slow pointers to locate mid.
    slow = fast = begin
    while fast.next != end and fast.next.next != end:
        fast = fast.next.next
        slow = slow.next

    # slow is at mid
    root = TreeNode(slow.val)
    root.left = build_bst(begin, slow)
    root.right = build_bst(slow.next, end)

    return root

