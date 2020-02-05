class Solution(object):
    def removeElements(self, head, val):
        """
        :type head: ListNode
        :type val: int
        :rtype: ListNode
        """
        if not head:
            return head

        dummy = ListNode(0)
        dummy.next = head
        pp = dummy
        p = head
        # How pp moves dependes on if we are going to delete the element.
        while p:
            if p.val == val:
                pp.next = p.next
            else:
                pp = p
            p = p.next

        return dummy.next
