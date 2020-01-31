class Solution(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        if len(prices) <= 1:
            return 0

        profit = 0
        # because we can complete transactions multiple times
        # for day prices: 1, 3, 5
        # buy at day 1 and sell at day 3 is exactly same as
        # buy at day 1 and sell at day 2, and then buy at day 2 and then sell at day 3
        # thus, we sell when prices[i] > prices[i-1], and we start to check from 2nd day,
        # because transaction at a single day is meaningless
        for i in range(1, len(prices)):
            if prices[i-1] < prices[i]:
                profit += prices[i] - prices[i-1]
        return profit


# another solution

class Solution(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        if len(prices) <= 1:
            return 0

        profit = 0
        buy_price = prices[0]
        # if prices[n-2] < prices[n-1], it won't trigger transaction calculation.
        # thus we need to do it separately.
        for i in range(len(prices) - 1):
            if prices[i] >= prices[i+1]:
                profit += prices[i] - buy_price
                buy_price = prices[i+1]

        if buy_price != prices[-1]:
            profit += prices[-1] - buy_price

        return profit
