# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 首先题目保证链表没有环形
# 分别遍历两个链表，拿到长度，并且为了方便最好保证 |headA| >= |headB|
# 算出长度差delta，先将pa往前走delta个结点
# 然后同时前进pa&pb；如果链表相交，则会存在某个结点使得pa==pb；否则不相交
class Solution(object):
    def getIntersectionNode(self, headA, headB):
        """
        :type head1, head1: ListNode
        :rtype: ListNode
        """
        if not headA or not headB:
            return None

        len_a = 0
        p = headA
        while p:
            len_a += 1
            p = p.next

        len_b = 0
        p = headB
        while p:
            len_b += 1
            p = p.next

        if len_a < len_b:
            len_a, len_b = len_b, len_a
            headA, headB = headB, headA

        delta = len_a - len_b

        pa = headA
        for _ in range(delta):
            pa = pa.next

        pb = headB
        while pa and pb:
            if pa == pb:
                return pa
            pa = pa.next
            pb = pb.next

        return None
