# Definition for a Node.
class Node:
    def __init__(self, x, next=None, random=None):
        self.val = int(x)
        self.next = next
        self.random = random

# 核心思路：
# 1. 遍历链表，对原链表每个结点都做一次duplicate，并且副本紧挨着原始结点
#    即 1->2->NUL => 1->1->2->2->NUL
#    并且不设置副本节点的random，因此此时某些副本节点尚未就绪，无法设置
# 2. 再遍历一次链表，设置副本的random；副本的random在原结点的random结点的next
#    不过需要注意可能原结点的random指向NUL
# 3. 第三次遍历链表，断开分离得到副本链表
class Solution(object):
    def copyRandomList(self, head):
        """
        :type head: Node
        :rtype: Node
        """
        if not head:
            return None

        # Duplicate nodes, and leave random of duplicates unset.
        p = head
        while p:
            pn = Node(p.val, p.next)
            p.next = pn
            p = pn.next

        # Set random of duplicates.
        p = head
        while p:
            if p.random:
                p.next.random = p.random.next
            p = p.next.next

        new_head = head.next

        # Separation.
        p = head
        while p:
            pn = p.next
            p.next = pn.next
            p = p.next
            if pn.next:
                pn.next = pn.next.next
            pn = pn.next

        return new_head


# Methods requires extra space for bookkeeping.

def copy_random_list(head):
    """
    :type head: Node
    :rtype: Node
    """
    if not head:
        return None

    # for old list
    node_idx_table = {}
    # for new list
    idx_node_list = []

    dummy = Node(0)
    pnew = dummy
    pold = head
    old_idx = 0
    while pold:
        node_idx_table[pold] = old_idx
        old_idx += 1
        pnew.next = Node(pold.val)
        pnew = pnew.next
        idx_node_list.append(pnew)
        pold = pold.next

    pold = head
    pnew = dummy.next
    while pnew:
        if pold.random:
            idx = node_idx_table[pold.random]
            pnew.random = idx_node_list[idx]
        pnew = pnew.next
        pold = pold.next

    return dummy.next
