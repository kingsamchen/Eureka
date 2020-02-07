# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思想
# 遍历链表，如果当前元素在G里，则检查是否 tagged: false -> true
# 如果是，则说明发现一个 component
# 元素不在G里的，说明脱离了component，reset tagged
class Solution(object):
    def numComponents(self, head, G):
        """
        :type head: ListNode
        :type G: List[int]
        :rtype: int
        """
        # Because values in linked list are unique,
        # thus no repeat access to an element in G could happen.
        table = set(G)

        tagged = False
        p = head
        count = 0
        # Find longest connected for a component as possible.
        while p:
            if p.val in table:
                if not tagged:
                    tagged = True
                    count += 1
            else:
                tagged = False

            p = p.next

        return count
