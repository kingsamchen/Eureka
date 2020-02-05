# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路：
# 因为如果有两个middle nodes选择右边一个，即ceilling
# 所以快慢指针的判断使用 while fast and fast.next 即可
class Solution(object):
    def middleNode(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        slow = fast = head
        while fast and fast.next:
            fast = fast.next.next
            slow = slow.next

        return slow
