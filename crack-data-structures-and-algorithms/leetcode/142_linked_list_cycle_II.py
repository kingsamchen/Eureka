# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def detect_cycle(head):
    """
    :type head: ListNode
    :rtype: ListNode
    """
    if not head:
        return None

    slow = fast = head
    has_cycle = False
    while fast.next and fast.next.next:
        fast = fast.next.next
        slow = slow.next
        if fast == slow:
            has_cycle = True
            break

    if not has_cycle:
        return None

    # A ------- B -------- +
    #           |          |
    #           |          |
    #           + -------- C
    # A: starting point B: cycle begins C: fast&slow meet point
    # When fast & slow met:
    # |fast| = |AB| + |BC| + |CB| + |BC|
    # |slow| = |AB| + |BC|
    # Becase we moved k steps and fast moved twice faster than slow
    # so, |fast| = 2|slow| => |AB| = |CB|
    # i.e length-from-start-to-cycle-beings == meet_point_to_cycle_begin
    # thus increment probe & slow one step each time, until they meet.
    probe = head
    while True:
        if slow == probe:
            return probe
        probe = probe.next
        slow = slow.next
