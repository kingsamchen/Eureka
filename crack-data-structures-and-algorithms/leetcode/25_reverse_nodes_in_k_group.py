# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

class Solution(object):
    def reverseKGroup(self, head, k):
        """
        :type head: ListNode
        :type k: int
        :rtype: ListNode
        """
        if not head:
            return None

        dummy = ListNode(0)
        dummy.next = head

        pp = dummy
        p = head
        while has_k_nodes(p, k):
            # extract-and-insert k-1 times
            for _ in range(1, k):
                pn = p.next
                p.next = pn.next
                pn.next = pp.next
                pp.next = pn
            # adjust pp & p
            pp = p
            p = p.next

        return dummy.next

def has_k_nodes(p, k):
    # start from curernt node(pointed by p)
    # check k times for k nodes.
    for _ in range(k):
        if not p:
            return False
        p = p.next
    return True