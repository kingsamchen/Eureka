# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def has_cycle(head):
    """
    :type head: ListNode
    :rtype: bool
    """
    if not head:
        return False
    slow = fast = head
    # fast goes forward twice fast as slow
    # if there is a cycle, fast will catch up with the slow.
    while fast.next and fast.next.next:
        fast = fast.next.next
        if fast == slow:
            return True
        slow = slow.next
    return False