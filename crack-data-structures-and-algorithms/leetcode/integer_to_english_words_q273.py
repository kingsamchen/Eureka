# 核心思路
# 分组打表：
# 1 ~ 19 的数字使用为一组；20，30，... 90 为一组；剩下的单独一组
# 利用递归，先判断是否由大数字的组组成，这一步结束之后，我们实际上只需要处理[1,99]这个范围
# 再根据是否 >= 20切分一次
# 三个注意点：
# 1）只要数字不等于0，英文标识中不会出现zero，所以前面打表，index=0的内容为空字符串
# 2）数字0需要单独处理，直接返回Zero
# 3）需要处理后缀的空字符串，因为有可能第二组或者第三组返回空字符串，所以需要把连接的空格也给去掉
class Solution(object):
    def numberToWords(self, num):
        """
        :type num: int
        :rtype: str
        """
        if num == 0:
            return 'Zero'

        table1 = [
            '', 'One', 'Two', 'Three', 'Four', 'Five', 'Six', 'Seven', 'Eight', 'Nine', 'Ten',
            'Eleven', 'Twelve', 'Thirteen', 'Fourteen', 'Fifteen', 'Sixteen', 'Seventeen', 'Eighteen', 'Nineteen'
        ]

        table2 = [
            '', '', 'Twenty', 'Thirty', 'Forty', 'Fifty', 'Sixty', 'Seventy', 'Eighty', 'Ninety'
        ]

        table3 = [
            (1000000000, 'Billion'), (1000000, 'Million'), (1000, 'Thousand'), (100, 'Hundred')
        ]

        return convert(num, table1, table2, table3)


def convert(num, t1, t2, t3):
    i = 0
    while i < len(t3):
        if num >= t3[i][0]:
            break
        i += 1

    if i < len(t3):
        part1 = num // t3[i][0]
        part2 = num % t3[i][0]
        w1 = convert(part1, t1, t2, t3) + ' ' + t3[i][1]
        w2 = convert(part2, t1, t2, t3)
        return w1 + ' ' + w2 if w2 != '' else w1

    # 0 < num < 100

    if num >= 20:
        part1 = num // 10
        part2 = num % 10
        w1 = t2[part1]
        w2 = t1[part2]
        return w1 + ' ' + w2 if w2 != '' else w1

    return t1[num]
