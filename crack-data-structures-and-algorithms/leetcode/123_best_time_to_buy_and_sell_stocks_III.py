class Solution(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        if len(prices) <= 1:
            return 0

        # for i >= 1 && k >= 1
        # mp[i,k] = max{mp[i-1,k], buy[i,k] + prices[i]}
        # buy[i,k] = max{buy[i-1,k], mp[i-1,k-1] - prices[i]}
        # mp[0,k] = 0; mp[i,0] = 0
        # buy[i,0] = 0; buy[0,1] = buy[0,2] = buy[0,k] = -prices[0]

        K = 2
        mp = [0] * (K + 1)
        buy = [0] * (K + 1)

        buy[1] = -prices[0]
        buy[2] = -prices[0]

        for i in range(1, len(prices)):
            for j in range(1, K + 1):
                buy[j] = max(buy[j], mp[j-1] - prices[i])
                mp[j] = max(mp[j], prices[i] + buy[j])

        return mp[-1]

# Without state compression.

class Solution2(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        if len(prices) <= 1:
            return 0

        K = 2
        mp = [[0] * (K + 1)] * len(prices)
        buy = [[0] * (K + 1)] * len(prices)

        buy[0][1] = -prices[0]
        buy[0][2] = -prices[0]

        for i in range(1, len(prices)):
            for j in range(1, K + 1):
                buy[i][j] = max(buy[i-1][j], mp[i-1][j-1] - prices[i])
                mp[i][j] = max(mp[i-1][j], prices[i] + buy[i][j])

        return mp[-1][-1]
