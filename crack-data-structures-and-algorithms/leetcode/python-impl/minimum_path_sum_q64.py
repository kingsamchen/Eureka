class Solution(object):
    def minPathSum(self, grid):
        """
        :type grid: List[List[int]]
        :rtype: int
        """
        if len(grid) == 0:
            return 0

        rcnt = len(grid)
        ccnt = len(grid[0])
        m = [[0] * ccnt for _ in range(rcnt)]
        m[0][0] = grid[0][0]

        for i in range(1, ccnt):
            m[0][i] = m[0][i-1] + grid[0][i]

        for i in range(1, rcnt):
            m[i][0] = m[i-1][0] + grid[i][0]

        for i in range(1, rcnt):
            for j in range(1, ccnt):
                m[i][j] = min(m[i-1][j], m[i][j-1]) + grid[i][j]

        return m[-1][-1]
