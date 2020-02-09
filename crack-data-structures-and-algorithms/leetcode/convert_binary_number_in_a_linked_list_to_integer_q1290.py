# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 因为链表是高位在前，意味着每遍历到一个新结点，结点值刚好处在低位
# 所以每遍历新的一位，num首先左移一位，然后与上当前结点的值
# 如果链表是低位在前，则可以考虑记索引，链表节点先左移在与
class Solution(object):
    def getDecimalValue(self, head):
        """
        :type head: ListNode
        :rtype: int
        """
        num = 0
        p = head
        while p:
            num <<= 1
            num |= p.val
            p = p.next

        return num
