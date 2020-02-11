class Solution(object):
    def generateParenthesis(self, n):
        """
        :type n: int
        :rtype: List[str]
        """
        result = []
        generate(n, n, '', result)
        return result


# 核心思路
# lparen / rparen 代表当前左括号或者右括号还剩下几个
# lparen > rparen，结果肯定不是 well-formed
# lparen == 0 and rparen == 0 表明一个组合已经生成
# 递归考虑添加左括号或者右括号或者都添加
def generate(lparen, rparen, s, result):
    if lparen > rparen:
        return

    if lparen == 0 and rparen == 0:
        result.append(s)
        return

    if lparen > 0:
        generate(lparen - 1, rparen, s + '(', result)

    if rparen > 0:
        generate(lparen, rparen - 1, s + ')', result)
