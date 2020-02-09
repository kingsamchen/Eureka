# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 将序数为偶数的结点串到一个单独的dummy链表里
# p指向序数为i的结点，如果i为偶数，则放到dummy链表里；这种情况下pp不动，p前移一个结点，因为有个节点被移除
# 始终保证pp是p的前继结点
# 记得 pe.next = None！
class Solution(object):
    def oddEvenList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        if not head or not head.next:
            return head

        dummy = ListNode(0)
        pe = dummy

        pp = head
        p = head.next
        i = 2
        while p:
            if i % 2 == 0:
                pe.next = p
                pe = pe.next
                pp.next = p.next
                p = p.next
            else:
                pp = p
                p = p.next

            i += 1

        pe.next = None
        pp.next = dummy.next

        return head
