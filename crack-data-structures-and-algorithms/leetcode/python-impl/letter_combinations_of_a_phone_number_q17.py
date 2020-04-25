# 核心思想：
# 每个数字都对应几个字母，递归往右拼接生成
# 递归基础条件：i == len(digits)，即已经检查完毕
class Solution(object):
    def letterCombinations(self, digits):
        """
        :type digits: str
        :rtype: List[str]
        """
        table = {
            '2': 'abc',
            '3': 'def',
            '4': 'ghi',
            '5': 'jkl',
            '6': 'mno',
            '7': 'pqrs',
            '8': 'tuv',
            '9': 'wxyz'
        }

        results = []
        combi(digits, 0, table, '', results)
        return results


def combi(digits, i, table, s, results):
    if i == len(digits):
        if s != '':
            results.append(s)
        return

    digit = digits[i]
    for c in table[digit]:
        combi(digits, i + 1, table, s + c, results)
