# Ensure get(), check() and release() all are O(1)
# 核心设计：
# 使用一个 map（或者这里用bitmap）来保存每个number对应的状态：unused/using
# 使用一个 queue 或者 stack 来保存可用的 number
class PhoneDirectory(object):

    def __init__(self, maxNumbers):
        """
        Initialize your data structure here
        @param maxNumbers - The maximum numbers that can be stored in the phone directory.
        :type maxNumbers: int
        """
        self._avail_nums = [i for i in range(maxNumbers)]
        self._used = [0] * maxNumbers

    def get(self):
        """
        Provide a number which is not assigned to anyone.
        @return - Return an available number. Return -1 if none is available.
        :rtype: int
        """
        if len(self._avail_nums) == 0:
            return -1
        num = self._avail_nums.pop()
        self._used[num] = 1
        return num


    def check(self, number):
        """
        Check if a number is available or not.
        :type number: int
        :rtype: bool
        """
        if number < 0 or number >= len(self._used):
            return False
        return self._used[number] == 0


    def release(self, number):
        """
        Recycle or release a number.
        :type number: int
        :rtype: None
        """
        if number < 0 or number >= len(self._used):
            return

        if self._used[number] == 0:
            return

        self._used[number] = 0
        self._avail_nums.append(number)
