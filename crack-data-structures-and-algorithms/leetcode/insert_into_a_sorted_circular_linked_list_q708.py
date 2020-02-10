# Definition for a Node.
class Node(object):
    def __init__(self, val=None, next=None):
        self.val = val
        self.next = next

# 核心思路
# 链表有序，表明大部分情况下 node_i <= node_i+1
# 但是：
# 1）如果有 node_i > node_i+1，则说明按顺序排列时，node_i为最后一个结点，node_i+1为头部节点
# 2）因为可能出现所有结点值都一样的情况，因此要避免循环判断，做法是判断 p.next 是不是题目给的 head，如果是
#    则p现在为最后一个head开始的链表的最后一个结点（可能不是顺序上的）
# 3）对于大部分情况，node_i <= x <= node_i+1满足时，就可以把x插入进去；
#    对于发生顺序跳跃的地方，只要满足 node_i <= x or x <= node_i+1即可
#    对于会发生环的，直接在head前一个结点停止即可
class Solution(object):
    def insert(self, head, insertVal):
        """
        :type head: Node
        :type insertVal: int
        :rtype: Node
        """
        if not head:
            head = Node(insertVal)
            head.next = head
            return head

        p = head
        pn = head.next
        # Don't loop; if pn == head then p is at the tail node
        while pn != head:
            if p.val <= insertVal and insertVal <= pn.val:
                break

            # new node might be the last node, or the first node in ordered list.
            if p.val > pn.val and (p.val <= insertVal or insertVal <= pn.val):
                break

            p = pn
            pn = pn.next

        node = Node(insertVal)
        node.next = pn
        p.next = node

        return head
