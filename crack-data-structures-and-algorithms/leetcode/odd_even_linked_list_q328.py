# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution(object):
    def oddEvenList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        if not head or not head.next:
            return head

        even = ListNode(0)
        pe = even

        i = 2
        pp = head
        p = head.next
        while p:
            if i % 2 == 0:
                pe.next = p
                pe = pe.next
                pp.next = p.next
                p = pp.next
            else:
                pp = p
                p = p.next
            i += 1

        pe.next = None
        pp.next = even.next

        return head
