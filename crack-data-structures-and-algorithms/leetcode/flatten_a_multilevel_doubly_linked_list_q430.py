"""
# Definition for a Node.
class Node(object):
    def __init__(self, val, prev, next, child):
        self.val = val
        self.prev = prev
        self.next = next
        self.child = child
"""

# 核心思路
# 需要一个递归函数，返回链表的 head & tail
# 遍历链表，如果某个结点有下层结点，就对下层节点进行flatten，拿到头尾结点之后衔接起来
# 要注意遍历链表时可能存在 p.next == NUL && p.child != NUL
class Solution(object):
    def flatten(self, head):
        """
        :type head: Node
        :rtype: Node
        """
        flatten(head)
        return head


# Return head & tail nodes
def flatten(head):
    pp = None
    p = head
    # Cannot use p and p.next, because the first layer may have only one node
    # and therefore, p.next might be None.
    while p:
        if p.child:
            child_head, child_tail = flatten(p.child)

            pn = p.next
            child_tail.next = pn
            if pn:
                pn.prev = child_tail

            p.child = None
            p.next = child_head
            child_head.prev = p

            pp = child_tail
            p = pn
        else:
            pp = p
            p = p.next

    return head, pp