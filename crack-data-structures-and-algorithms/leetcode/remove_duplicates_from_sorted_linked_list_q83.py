# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路：
# 因为要保留一个所以头结点不会被移除，不需要dummy
# 如果 p.next.val == p.val，则立即跳过p.next这个节点；这样p.next就变成了下下一个节点
# 同时因为已经去掉了一个重复结点，p不需要前移
# 如此重复判断即可
class Solution(object):
    def deleteDuplicates(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        if not head:
            return None

        # First node will never be removed, thus no need for dummy.
        # Move forward when the adjacent node is not a duplicate.
        p = head
        while p and p.next:
            if p.val == p.next.val:
                p.next = p.next.next
            else:
                p = p.next
        return head