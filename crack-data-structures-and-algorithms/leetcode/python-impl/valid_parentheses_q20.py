# 核心思路：
# 使用stack来保存左括号，遇到右括号时弹栈匹配
# 用一个map来做左右括号的关联，简化实现
class Solution(object):
    def isValid(self, s):
        """
        :type s: str
        :rtype: bool
        """
        table = {'(': ')', '[': ']', '{': '}'}
        bk = []
        for c in s:
            if c in table:
                bk.append(c)
            else:
                if len(bk) == 0 or table[bk.pop()] != c:
                    return False
        return len(bk) == 0
