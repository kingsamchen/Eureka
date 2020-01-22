class Solution(object):
    def minimumTotal(self, triangle):
        """
        :type triangle: List[List[int]]
        :rtype: int
        """
        if len(triangle) == 0:
            return 0

        # our dp bookkeeping, copy of last row
        path_sum = triangle[-1][:]

        # bottom up strategy.
        # dp[m][n] = min(dp[m + 1][n], dp[m + 1][n + 1]) + triangle[m][n]
        # and since we only need i-th row's data when we are processing (i-1)-th row
        # O(n) extra space is totally fit.
        for i in range(len(triangle) - 2, -1, -1):
            for j in range(len(triangle[i])):
                path_sum[j] = min(path_sum[j], path_sum[j+1]) + triangle[i][j]

        return path_sum[0]
