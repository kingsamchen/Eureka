# 核心思路
# 利用bk作为单调栈，如果遍历时ch小于bk栈顶元素，则不断弹出
# 直到1）弹出次数已经达到k上限 2）栈空
# 结束遍历后如果k有余且栈不为空，继续弹栈
# 栈低保存最后数的高位，栈顶为低位，因此保证栈从上到下单调递减
# 可以令最后结果最小
class Solution(object):
    def removeKdigits(self, num, k):
        """
        :type num: str
        :type k: int
        :rtype: str
        """
        bk = []
        for c in num:
            while k > 0 and len(bk) > 0 and int(c) < int(bk[-1]):
                bk.pop()
                k -= 1
            if len(bk) > 0 or c != '0':
                bk.append(c)

        while k > 0 and len(bk) > 0:
            bk.pop()
            k -= 1

        return ''.join(bk) if len(bk) > 0 else '0'
