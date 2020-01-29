class Solution(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        if len(prices) <= 1:
            return 0

        rest = [0] * len(prices)
        hold = [0] * len(prices)
        hold[0] = -prices[0]
        sold = [0] * len(prices)

        for i in range(1, len(prices)):
            rest[i] = max(rest[i - 1], sold[i - 1])
            hold[i] = max(hold[i - 1], rest[i - 1] - prices[i])
            sold[i] = hold[i - 1] + prices[i]

        return max(rest[-1], sold[-1])


# With space compression

class Solution2(object):
    def maxProfit(self, prices):
        """
        :type prices: List[int]
        :rtype: int
        """
        if len(prices) <= 1:
            return 0
        rest = 0
        hold = -prices[0]
        sold = 0
        for i in range(1, len(prices)):
            old_sold = sold
            sold = hold + prices[i]
            hold = max(hold, rest - prices[i])
            rest = max(rest, old_sold)
        return max(rest, sold)
