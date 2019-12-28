# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def get_intersection_node(headA, headB):
    """
    :type head1, head1: ListNode
    :rtype: ListNode
    """
    if not headA or not headB:
        return None

    i = 0
    ia = headA
    while ia.next:
        ia = ia.next
        i += 1

    j = 0
    jb = headB
    while jb.next:
        jb = jb.next
        j += 1

    # If |A| = |B|, then the meet point of A & B is the intersection.
    # Thus we make the longer list move first.
    short, large = None, None
    delta = 0
    if i > j:
        large = headA
        short = headB
        delta = i - j
    else:
        large = headB
        short = headA
        delta = j - i

    while delta > 0:
        large = large.next
        delta -= 1

    while large:
        if large == short:
            return large
        large = large.next
        short = short.next
    return None
