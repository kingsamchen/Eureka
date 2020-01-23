class Solution(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        if len(prices) == 0:
            return 0

        profit = 0
        # avoid using INT_MAX
        buy_price = prices[0]
        # for each price we update our buy_price if necessary, and calculate profit so far.
        for p in prices:
            if p < buy_price:
                buy_price = p
            profit = max(profit, p - buy_price)

        return profit
