# 核心思路
# 因为罗马数字的拼写规则，如果value(s[i]) < value(s[i+1])
# 那么数值需要 -value(s[i]) + value(s[i+1])
# 可以利用这点一次遍历完成
class Solution(object):
    def romanToInt(self, s):
        """
        :type s: str
        :rtype: int
        """
        alphabet = {
            'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000
        }
        value = 0
        for i in range(len(s)):
            if i < len(s) - 1 and alphabet[s[i]] < alphabet[s[i+1]]:
                value -= alphabet[s[i]]
            else:
                value += alphabet[s[i]]
        return value
