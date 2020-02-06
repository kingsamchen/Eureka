# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 1. 遍历链表并将链表形成环，同时得到链表长度 cnt；注意此时指针p应该指向尾结点
# 2. 因为题目要往右转k个节点，等价于往左转 cnt - k % cnt个结点；如果题目直接说往左，那么省掉这一步转换
# 3. 往左转L个结点，等于从头节点head开始往右跳L次的结点为新头结点，而为了断开链表，则从head跳L-1次的结点
#    为尾结点
# 4. 因为我们指针p现在处于尾结点，因此直接往右跳L次刚好在新尾结点；从此处断开即可
class Solution(object):
    def rotateRight(self, head, k):
        """
        :type head: ListNode
        :type k: int
        :rtype: ListNode
        """
        if not head or not head.next:
            return head

        # make it a circle and count nodes
        cnt = 1
        p = head
        while p.next:
            p = p.next
            cnt += 1
        p.next = head


        # p now points to the original tail
        # now make it point to new tail
        # NOTE: if we move cnt steps, we will stop at the same position.
        # thus rotate right k % cnt means k % cnt nodes before the original tail.
        move = cnt - k % cnt
        for _ in range(move):
            p = p.next

        new_head = p.next
        p.next = None

        return new_head
