# 设计核心：
# Map 和 双向链表
# map存储结构为 <key, node-reference>, 其中 node-reference 为链表中某个节点的引用，可以访问到这个节点
# 链表的每个元素需要保存 (key, value), key和map中的key一样，主要为了方便在map中回溯查询；value就是
# 保存的 entry-value
class LRUCache(object):

    def __init__(self, capacity):
        """
        :type capacity: int
        """
        self._cap = capacity
        self._size = 0

        self._table = {}

        self._head = Node(0, -1)
        self._tail = Node(0, -1)
        self._head.next = self._tail
        self._tail.prev = self._head


    # 取元素后需要将结点放到最后，更新结点的TTL
    def get(self, key):
        """
        :type key: int
        :rtype: int
        """
        if key not in self._table:
            return -1

        node = self._table[key]
        node.prev.next = node.next
        node.next.prev = node.prev
        node.next = self._tail
        node.prev = self._tail.prev
        self._tail.prev.next = node
        self._tail.prev = node

        return node.val

    # 如果key已经存在，得更新key和TTL，直接用 get 获取一遍就OK；同时 size 不变
    # 否则就要插新元素，可能需要先 evict 过期元素腾空间
    def put(self, key, value):
        """
        :type key: int
        :type value: int
        :rtype: None
        """
        if self.get(key) != -1:
            self._table[key].val = value
            return

        if self._size == self._cap:
            victim = self._head.next
            del self._table[victim.key]
            self._head.next = victim.next
            victim.next.prev = self._head
            self._size -= 1

        node = Node(key, value, self._tail.prev, self._tail)
        node.prev.next = node
        self._tail.prev = node

        self._table[key] = node

        self._size += 1


class Node:
    def __init__(self, key, value, prev=None, next=None):
        self.key = key
        self.val = value
        self.prev = prev
        self.next = next
