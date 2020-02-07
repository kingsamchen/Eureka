class Node(object):
    def __init__(self, x):
        self.val = x
        self.next = None

class MyLinkedList(object):

    def __init__(self):
        """
        Initialize your data structure here.
        """
        self._size = 0
        self._dummy = Node(0)

    def get(self, index):
        """
        Get the value of the index-th node in the linked list. If the index is invalid, return -1.
        :type index: int
        :rtype: int
        """
        if index >= self._size or index < 0:
            return -1

        i = 0
        p = self._dummy
        while i <= index:
            p = p.next
            i += 1
        return p.val


    def addAtHead(self, val):
        """
        Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list.
        :type val: int
        :rtype: None
        """
        node = Node(val)
        node.next = self._dummy.next
        self._dummy.next = node
        self._size += 1


    def addAtTail(self, val):
        """
        Append a node of value val to the last element of the linked list.
        :type val: int
        :rtype: None
        """
        p = self._dummy
        while p.next:
            p = p.next
        p.next = Node(val)
        self._size += 1

    def addAtIndex(self, index, val):
        """
        Add a node of value val before the index-th node in the linked list. If index equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted.
        :type index: int
        :type val: int
        :rtype: None
        """
        if index > self._size or index < 0:
            return

        p = self._dummy
        i = 0
        while i < index:
            p = p.next
            i += 1

        node = Node(val)
        node.next = p.next
        p.next = node

        self._size += 1

    def deleteAtIndex(self, index):
        """
        Delete the index-th node in the linked list, if the index is valid.
        :type index: int
        :rtype: None
        """
        if index < 0 or index >= self._size:
            return

        pp = self._dummy
        p = pp.next
        i = 0
        while i < index:
            i += 1
            pp = p
            p = p.next

        pp.next = p.next

        self._size -= 1
