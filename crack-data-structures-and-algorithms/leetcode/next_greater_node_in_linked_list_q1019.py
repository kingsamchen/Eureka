# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

class Solution(object):
    def nextLargerNodes(self, head):
        """
        :type head: ListNode
        :rtype: List[int]
        """
        # linked-list to array, making it convenient for reverse iteration.
        vals = []
        while head:
            vals.append(head.val)
            head = head.next

        # stack maintains values such that they are in decreasing order from bottom to top
        # and stack.top() > val being always true.
        stack = []
        results = [0] * len(vals)
        for i in range(len(vals) - 1, -1, -1):
            val = vals[i]
            while len(stack) > 0 and stack[-1] <= val:
                stack.pop()
            results[i] = stack[-1] if len(stack) > 0 else 0
            stack.append(val)

        return results
