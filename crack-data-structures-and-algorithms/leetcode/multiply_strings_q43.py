# 核心思路
# 常规的乘法运算逻辑，从低位到高位相乘求和（原字符串需要反转）
# 被乘数长度要不小于乘数，并且每一轮乘数数字结束后需要处理 carry
# 写结果数组的时候要注意不要越界，因为有可能需要先扩容
# 结果数组是低位数字在前，转换成最后结果需要再处理一下
class Solution(object):
    def multiply(self, num1, num2):
        """
        :type num1: str
        :type num2: str
        :rtype: str
        """
        if len(num1) < len(num2):
            num1, num2 = num2, num1

        if num2 == '0':
            return '0'

        result = []

        rnum1 = num1[::-1]
        rnum2 = num2[::-1]
        for i in range(len(rnum2)):
            carry = 0
            for j in range(len(rnum1)):
                val = int(rnum2[i]) * int(rnum1[j]) + carry
                carry = val / 10
                val %= 10
                idx = i + j
                if idx >= len(result):
                    result.append(val)
                else:
                    result[idx] += val
                    if result[idx] >= 10:
                        result[idx] %= 10
                        carry += 1

            while carry > 0:
                result.append(carry % 10)
                carry /= 10

        s = ''
        for num in result[::-1]:
            s += str(num)
        return s