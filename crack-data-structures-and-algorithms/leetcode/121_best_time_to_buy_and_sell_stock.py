class Solution(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        profit = 0
        sell_price = 0
        # backtrace.
        for i in range(len(prices) - 1, -1, -1):
            if prices[i] > sell_price:
                sell_price = prices[i]
            else:
                profit = max(profit, sell_price - prices[i])

        return profit
