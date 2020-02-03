# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None


class Solution(object):
    def sortList(self, head):
        """
        :type head: ListNode
        :rtype: ListNode
        """
        return merge_sort_list(head)

def merge_sort_list(head):
    if not head or not head.next:
        return head

    slow = fast = head
    while fast.next and fast.next.next:
        fast = fast.next.next
        slow = slow.next

    # Split into two lists.
    # Why head2 starts from the next node of mid(slow)?
    # Assume we have only two nodes, A -> B -> ^
    # The strategy we use here eseentially is like floor((l + r) / 2), which
    # always stucks on A if we make mid the head.
    # Logically, mid with floor strategy makes it the **last element** of the first part.
    head2 = slow.next
    slow.next = None

    l1 = merge_sort_list(head)
    l2 = merge_sort_list(head2)

    return merge_lists(l1, l2)


def merge_lists(l1, l2):
    # Introduce dummy node to simplify merge.
    # No need to check l1 & l2 up front
    dummy = ListNode(0)
    p = dummy

    while l1 and l2:
        if l1.val < l2.val:
            p.next = l1
            l1 = l1.next
        else:
            p.next = l2
            l2 = l2.next

        p = p.next

    if l1:
        p.next = l1

    if l2:
        p.next = l2

    return dummy.next
