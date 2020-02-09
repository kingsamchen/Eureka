# Definition for singly-linked list.
class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None

# 核心思路
# 1. 因为题目要求的是每个元素之后的第一个更大的元素，所以求解需要把序列逆过来求解
#    可以选择反转链表，也可以把元素存到一个list里
# 2. 需要一个辅助数组，新元素添加在尾部，并且保证ary.back() > val
#    操作上栈就可以满足，但是并不一定真的需要一个栈；只要能在尾部增减元素即可
# 3. 逆序遍历时，保证 ary.back() > val，如果不满足就从尾部删除元素；这个尾部元素就是val的next greater value
#    然后将val加入辅助数组的尾部，因为这个val有可能是其他元素的next greater value
#    上面操作保证了这个辅助数组的元素时递减的
# 4. 记得考虑辅助数组为空的边界条件
# 理解辅助数组：
# 1）每个元素的next greater value是从辅助数组出的，所以辅助数组本身需要保存逆序遍历的元素的值
# 2）因为我们是逆序遍历，越后面存入辅助数组的元素越要先判断，否则违背了next greater value定义；同时要考虑
#    如果 ary.back() 并不是遍历遇到元素的结果，即 ary.back() <= val，需要将其弹出，露出前面遍历过，更大的元素
#   所以辅助数组一定是递减顺序
class Solution(object):
    def nextLargerNodes(self, head):
        """
        :type head: ListNode
        :rtype: List[int]
        """
        # linked-list to array, making it convenient for reverse iteration.
        vals = []
        while head:
            vals.append(head.val)
            head = head.next

        # stack maintains values such that they are in decreasing order from bottom to top
        # and stack.top() > val being always true.
        stack = []
        results = [0] * len(vals)
        for i in range(len(vals) - 1, -1, -1):
            val = vals[i]
            while len(stack) > 0 and stack[-1] <= val:
                stack.pop()
            results[i] = stack[-1] if len(stack) > 0 else 0
            stack.append(val)

        return results
