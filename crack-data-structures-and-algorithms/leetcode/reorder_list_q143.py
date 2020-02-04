# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


# 整体思路：
# 1. 用快慢指针找到中点，切断得到两个list，且中点后一个节点为第二个list的头节点
# 2. 将第二个list整体反转
# 3. 将第二个list的节点间隙插入第一个list
class Solution(object):
    def reorderList(self, head):
        """
        :type head: ListNode
        :rtype: None Do not return anything, modify head in-place instead.
        """
        if not head or not head.next:
            return head

        slow = fast = head
        while fast.next and fast.next.next:
            fast = fast.next.next
            slow = slow.next

        # Cut off and we now have two lists
        head2 = slow.next
        slow.next = None

        # Reverse the 2nd list
        pp = None
        p = head2
        while p:
            pn = p.next
            p.next = pp
            pp = p
            p = pn

        head2 = pp

        # Try to insert nodes in 2nd list into the first list.
        p1 = head
        p2 = head2
        while p1 and p2:
            pn2 = p2.next
            p2.next = p1.next
            p1.next = p2
            p1 = p2.next
            p2 = pn2


# 另外一种比较差的思路是每一轮迭代 reverse 后面所有的节点
# 例如： 1->2->3->4 => 1->4->3->2 => 1->4->2->3
# 时间复杂度 O(n^2)
# 会超时但是可以用在第一回合的 brute-force
