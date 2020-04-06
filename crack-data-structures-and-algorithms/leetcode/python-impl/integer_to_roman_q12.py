# 核心思路
# 先将类似4、9这样的数值打上表，然后逆序向下处理即可
class Solution(object):
    def intToRoman(self, num):
        """
        :type num: int
        :rtype: str
        """
        table = [
            (1, 'I'), (4, 'IV'), (5, 'V'), (9, 'IX'),
            (10, 'X'), (40, 'XL'), (50, 'L'), (90, 'XC'),
            (100, 'C'), (400, 'CD'), (500, 'D'), (900, 'CM'),
            (1000, 'M')
        ]

        roman = ''

        i = len(table) - 1
        while i >= 0:
            if num >= table[i][0]:
                n = table[i][0]
                k = num // n
                roman += table[i][1] * k
                num -= n * k

            i -= 1

        return roman
