# 核心思路
# 用dp[i]来记录字符串s[0:i]（右边取不到）是否可以由字典中的字组成
# dp[0] == s[0:0] == 空串，因此dp[1] 是我们的基础case
# 另外长度上 len(dp) = len(s) + 1
# 转移方程： dp[i] = dp[j] && (s[j:i] in dict), 0 <= j < i < len(s) + 1
class Solution(object):
    def wordBreak(self, s, wordDict):
        """
        :type s: str
        :type wordDict: List[str]
        :rtype: bool
        """
        word_table = set(wordDict)
        dp = [False] * (len(s) + 1)
        dp[0] = True
        for i in range(1, len(s) + 1):
            for j in range(i):
                if dp[j] and (s[j:i] in word_table):
                    dp[i] = True
                    break
        return dp[-1]
