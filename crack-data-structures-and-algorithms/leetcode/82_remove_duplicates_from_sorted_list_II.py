# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

class Solution(object):
    def deleteDuplicates(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        if not head:
            return None

        dummy = ListNode(0)
        dummy.next = head
        pp = dummy
        p = head
        # Use dummy node to resolve head node issue: duplicates happen at head
        # Now
        # dummy -> node1 -> node2 -> node3 ... -> noden
        #            ^
        #            |
        #           head
        # at first, we make dummy.next = head and prev = dummy
        # whenever no adjacent duplicates, move forward prev
        # otherwise, prev.next = new_node_pos
        # In the 1st case, p = probe may make p None, thus both p and p.next must be checked.
        while p and p.next:
            if p.next.val == p.val:
                # probe points to unchecked node.
                probe = p.next.next
                while probe and probe.val == p.val:
                    probe = probe.next
                pp.next = probe
                p = probe
            else:
                pp = p
                p = p.next

        return dummy.next
