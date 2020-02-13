# 核心思路
# 对于负数，需要首先转换成正数，否则默认floor取整的除法会导致 -1 / 10 = -1这种情况的出现
# 再注意一下初始val=0的更新条件
class Solution(object):
    def reverse(self, x):
        """
        :type x: int
        :rtype: int
        """
        val = 0

        sign = False
        if x < 0:
            sign = True
            x = -x

        while x != 0:
            r = x % 10
            x /= 10
            if val == 0:
                val = r
            else:
                val = val * 10 + r

        if sign:
            val = -val

        # Handling overflow case.
        INT_MIN = -(1 << 31)
        INT_MAX = (1 << 31) - 1
        if val < INT_MIN:
            val = 0
        elif val > INT_MAX:
            val = 0

        return val
