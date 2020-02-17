# 可以认为是26进制数->10进制
class Solution(object):
    def titleToNumber(self, s):
        """
        :type s: str
        :rtype: int
        """
        number = 0
        for i in range(len(s)):
            exp = 26 ** (len(s) - 1 - i)
            number += seq(s[i]) * exp

        return number

def seq(ch):
    return ord(ch) - 64
