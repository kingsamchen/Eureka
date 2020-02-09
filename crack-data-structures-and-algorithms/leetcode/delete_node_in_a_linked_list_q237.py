# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 题目约束了至少有两个结点并且不会删除尾结点
# 所以用偷天换日法：将p.val值设为p.next.val，然后删除p.next
# 编程之美上也有着题目
class Solution(object):
    def deleteNode(self, node):
        """
        :type node: ListNode
        :rtype: void Do not return anything, modify node in-place instead.
        """
        pn = node.next
        node.val = pn.val
        node.next = pn.next
