# 核心思路
# 因为A-Z对应的是1-26，所以没法直接mod 26。
# 所以利用n-1做线性变换，将结果映射到0-25
class Solution(object):
    def convertToTitle(self, n):
        """
        :type n: int
        :rtype: str
        """
        s = ''
        while n > 0:
            base = (n - 1) % 26 + 1
            s += enc(base)
            n = (n - 1) // 26

        return s[::-1]


# returns A ~ Z for 1 ~ 26 respectively
def enc(num):
    return chr(num + 64)
