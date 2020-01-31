# -*- coding: utf-8 -*-
# 0xCCCCCCCC


# Definition for a Node.
class Node:
    def __init__(self, x, next=None, random=None):
        self.val = int(x)
        self.next = next
        self.random = random


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

'''
without extra space
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        Node *cur = head;
        while (cur) {
            Node *t = new Node(cur->val, nullptr, nullptr);
            t->next = cur->next;
            cur->next = t;
            cur = t->next;
        }
        cur = head;
        while (cur) {
            if (cur->random) cur->next->random = cur->random->next;
            cur = cur->next->next;
        }
        cur = head;
        Node *res = head->next;
        while (cur) {
            Node *t = cur->next;
            cur->next = t->next;
            if (t->next) t->next = t->next->next;
            cur = cur->next;
        }
        return res;
    }
}; 
'''
