# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def delete_duplicates(head):
    """
    :type head: ListNode
    :rtype: ListNode
    """
    # Move forward when the adjacent node is not a duplicate.
    p = head
    while p and p.next:
        if p.val == p.next.val:
            p.next = p.next.next
        else:
            p = p.next
    return head
