# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

class Solution(object):
    def reverseList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        # Special traits:
        # This question is whole-list reverse, thus
        # * No need for dummy node, since the final value of pp is our new head.
        # * No need to handle special cases like head == None or only a single node,
        #   because our loop invariant takes care of it well.
        pp = None
        p = head
        while p:
            pn = p.next
            p.next = pp
            pp = p
            p = pn

        return pp

# Use recursion

class SolutionRecursion(object):
    def reverseList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        tail, root = reverse(head)
        return root

def reverse(head):
    if not head or not head.next:
        return head, head

    pp, root = reverse(head.next)
    pp.next = head
    head.next = None
    return head, root
