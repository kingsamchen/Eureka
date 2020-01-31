class Solution(object):
    def maxProfit(self, prices, fee):
        """
        :type prices: List[int]
        :type fee: int
        :rtype: int
        """
        if len(prices) <= 1:
            return 0
        sold = [0] * len(prices)
        hold = [0] * len(prices)
        hold[0] = -prices[0]
        for i in range(1, len(prices)):
            hold[i] = max(hold[i-1], sold[i-1] - prices[i])
            sold[i] = max(sold[i-1], hold[i-1] + prices[i] - fee)
        return max(hold[-1], sold[-1])

# with space compression

class Solution2(object):
    def maxProfit(self, prices, fee):
        """
        :type prices: List[int]
        :type fee: int
        :rtype: int
        """
        if len(prices) <= 1:
            return 0
        sold = 0
        hold = -prices[0]
        for i in range(1, len(prices)):
            old_hold = hold
            hold = max(hold, sold - prices[i])
            sold = max(sold, old_hold + prices[i] - fee)
        return max(hold, sold)