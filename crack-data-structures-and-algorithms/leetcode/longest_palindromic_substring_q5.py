# 核心思路
# 使用DP
# dp[i,j] (i <= j) 表示 s[i,j]是否存在回文，那么则有
# dp[i,j] = 1, if i == j
# dp[i,j] = s[i] == s[j], if i + 1 == j
# dp[i,j] = s[i] == s[j] && dp[i+1,j-1] == 1, if j - i > 1
# 注意，因为第三个递推式里出现dp[i,j]由dp[i+1,j-1]导出的情况，所以代码实现上逆序遍历
class Solution(object):
    def longestPalindrome(self, s):
        """
        :type s: str
        :rtype: str
        """
        cnt = len(s)
        dp = [[0] * cnt for _ in range(cnt)]

        result = ''
        longest = 0

        for i in range(cnt - 1, -1, -1):
            for j in range(cnt - 1, i - 1, -1):
                if i == j:
                    dp[i][j] = 1
                elif i + 1 == j:
                    dp[i][j] = 1 if s[i] == s[j] else 0
                else:
                    dp[i][j] = 1 if s[i] == s[j] and dp[i + 1][j - 1] == 1 else 0

                if dp[i][j] == 1 and j - i + 1 > longest:
                    longest = j - i + 1
                    result = s[i:j + 1]

        return result

# 也有 manacher 算法
