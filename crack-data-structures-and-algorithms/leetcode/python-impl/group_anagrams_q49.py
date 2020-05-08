# 核心思路
# 将每个str排序之后作为基准，用map保存基准一样的字符串
class Solution(object):
    def groupAnagrams(self, strs):
        """
        :type strs: List[str]
        :rtype: List[List[str]]
        """
        table = {}
        for s in strs:
            base = ''.join(sorted(s))
            if base not in table:
                table[base] = [s]
            else:
                table[base].append(s)

        return list(table.values())
