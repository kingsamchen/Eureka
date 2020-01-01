class Solution(object):
    def removeElements(self, head, val):
        """
        :type head: ListNode
        :type val: int
        :rtype: ListNode
        """
        if not head:
            return None

        dummy = ListNode(0)
        dummy.next = head
        pp = dummy
        p = head
        while p:
            if p.val == val:
                pp.next = p.next
                p = pp.next
            else:
                pp = p
                p = p.next

        return dummy.next
