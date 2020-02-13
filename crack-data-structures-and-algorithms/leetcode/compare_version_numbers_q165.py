# 核心思路
# 按照.将版本号分割成int数组，然后逐个比较
# 如果两个版本数组长度不一致，则长的数组多余的版本数字只要有非0就更大
# 这题很依赖语言提供的特性，例如字符串split，string to int等
class Solution(object):
    def compareVersion(self, version1, version2):
        """
        :type version1: str
        :type version2: str
        :rtype: int
        """
        comp1 = split_ver(version1)
        num1 = len(comp1)
        comp2 = split_ver(version2)
        num2 = len(comp2)

        for i in range(min(num1, num2)):
            if comp1[i] < comp2[i]:
                return -1
            elif comp1[i] > comp2[i]:
                return 1

        if num1 > num2:
            for c in comp1[num2:]:
                if c > 0:
                    return 1
        elif num1 < num2:
            for c in comp2[num1:]:
                if c > 0:
                    return -1

        return 0


def split_ver(ver):
    parts = ver.split('.')
    return [int(v) for v in parts]
