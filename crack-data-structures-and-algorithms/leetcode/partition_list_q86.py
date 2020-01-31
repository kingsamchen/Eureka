# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

class Solution(object):
    def partition(self, head, x):
        """
        :type head: ListNode
        :type x: int
        :rtype: ListNode
        """
        if not head:
            return None

        # We move nodes whose value >= x to another list, whose tail node indicated by pr
        # When we done iteration, we splice two lists together.

        dummy_l = ListNode(0)
        dummy_l.next = head

        dummy_r = ListNode(0)
        pr = dummy_r

        pp = dummy_l
        p = head
        while p:
            if p.val < x:
                pp = p
                p = p.next
            else:
                # move forward pr
                pr.next = p
                pr = pr.next
                # extract
                pp.next = p.next
                p.next = None
                p = pp.next

        pp.next = dummy_r.next

        return dummy_l.next

# Another solution.

def partition(head, x):
    """
    :type head: ListNode
    :type x: int
    :rtype: ListNode
    """
    if not head:
        return None

    dummy = ListNode(0)
    dummy.next = head
    pp = dummy
    p = dummy
    while p.next:
        pn = p.next
        if pn.val < x:
            if pp.next == pn:
                pp = pp.next
                p = p.next
            else:
                p.next = pn.next
                pn.next = pp.next
                pp.next = pn
                pp = pn
        else:
            p = p.next

    return dummy.next
