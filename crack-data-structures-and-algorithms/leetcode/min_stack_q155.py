# 核心思路
# 使用两个栈：一个保存栈元素，一个记录当前最小元素的历史
class MinStack(object):

    def __init__(self):
        """
        initialize your data structure here.
        """
        self._stack = []
        self._min_list = []


    def push(self, x):
        """
        :type x: int
        :rtype: None
        """
        self._stack.append(x)
        if len(self._min_list) == 0 or x <= self._min_list[-1]:
            self._min_list.append(x)


    def pop(self):
        """
        :rtype: None
        """
        x = self._stack.pop()
        if self._min_list[-1] == x:
            self._min_list.pop()


    def top(self):
        """
        :rtype: int
        """
        return self._stack[-1]


    def getMin(self):
        """
        :rtype: int
        """
        return self._min_list[-1]
