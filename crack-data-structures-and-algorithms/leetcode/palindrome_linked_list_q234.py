# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

# 核心思路：
# 利用快慢指针定位中点并将链表一分为二
# 将链表2反转
# 同时遍历链表1和链表2，如果链表回文，则对应结点值一致
class Solution(object):
    def isPalindrome(self, head):
        """
        :type head: ListNode
        :rtype: bool
        """
        if not head or not head.next:
            return True

        slow = fast = head
        while fast.next and fast.next.next:
            fast = fast.next.next
            slow = slow.next

        # Cut off for two lists.
        # Head of the 2nd list is next of slow(mid).
        head2 = slow.next
        slow.next = None

        # Reverse the 2nd one
        pp = None
        p = head2
        while p:
            pn = p.next
            p.next = pp
            pp = p
            p = pn

        head2 = pp

        # 2nd list is always shorter than 1st.
        p1 = head
        p2 = head2
        while p2:
            if p1.val != p2.val:
                return False
            p1 = p1.next
            p2 = p2.next

        return True
