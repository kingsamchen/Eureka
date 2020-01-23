# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


class Solution(object):
    def rotateRight(self, head, k):
        """
        :type head: ListNode
        :type k: int
        :rtype: ListNode
        """
        if not head or not head.next:
            return head

        # make it a circle and count nodes
        cnt = 1
        p = head
        while p.next:
            p = p.next
            cnt += 1
        p.next = head


        # p now points to the original tail
        # now make it point to new tail
        # NOTE: if we move cnt steps, we will stop at the same position.
        # thus rotate right k % cnt means k % cnt nodes before the original tail.
        move = cnt - k % cnt
        for _ in range(move):
            p = p.next

        new_head = p.next
        p.next = None

        return new_head
