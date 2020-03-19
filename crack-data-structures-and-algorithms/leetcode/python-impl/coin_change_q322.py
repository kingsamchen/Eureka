# 核心思路
# 使用DP，状态转移方程
# dp[i] 表示金额为i时最少需要的零钱数
# c1, c2, c3 ... cn 代表零钱的面额
# dp[i] = min(dp[i-c1] + 1, dp[i-c2] + 1, ... dp[i-cn] + 1)
# dp[0] = 0
# 注意默认值 amount+1 同时也作为哨兵
class Solution(object):
    def coinChange(self, coins, amount):
        """
        :type coins: List[int]
        :type amount: int
        :rtype: int
        """
        dp = [amount+1] * (amount + 1)
        dp[0] = 0
        for i in range(1, amount + 1):
            for c in coins:
                delta = i - c
                if delta < 0:
                    continue
                # Update dp[i] to minimum if possible.
                dp[i] = min(dp[i], dp[delta] + 1)

        return dp[-1] if dp[-1] != (amount + 1) else -1
