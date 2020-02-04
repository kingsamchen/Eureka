# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


class Solution(object):
    def addTwoNumbers(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        if not l1 and not l2:
            return None

        # Make l1 always being the longer list.
        len1 = list_len(l1)
        len2 = list_len(l2)
        if len1 < len2:
            l1, l2 = l2, l1
            len1, len2 = len2, len1

        dummy = ListNode(0)
        # p points to currently handled node
        # pnc points to the nearest node whose value is not 9 (won't carry)
        p = pnc = dummy

        # Handle length-diff first.
        for _ in range(len1 - len2):
            p.next = ListNode(l1.val)
            p = p.next
            if p.val != 9:
                pnc = p
            l1 = l1.next

        # No carry-flag needed because we handle cf in-place.
        while l1:
            val = l1.val + l2.val
            if val > 9:
                # increment pnc's value, consuming the carry.
                pnc.val += 1
                # zero subsequent nodes whose value is 9
                while pnc.next:
                    pnc = pnc.next
                    pnc.val = 0
                # The new node must cannot be 9, so make pnc point to it.
                val %= 10
                p.next = ListNode(val)
                p = p.next
                pnc = p
            else:
                p.next = ListNode(val)
                p = p.next
                if p.val != 9:
                    pnc = p

            l1 = l1.next
            l2 = l2.next

        return dummy if dummy.val == 1 else dummy.next


def list_len(head):
    cnt = 0
    while head:
        cnt += 1
        head = head.next
    return cnt
