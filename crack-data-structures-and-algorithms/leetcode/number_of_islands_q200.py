# 核心思路：
# 本质是计算矩阵里的连通区域（标记为1）；经典的 DFS 应用
# 直接用 DFS 遍历一块完整区域；用 visited 辅助矩阵记录已经遍历过的区域
# 最后遍历的区域数就是要求的结果
class Solution(object):
    def numIslands(self, grid):
        """
        :type grid: List[List[str]]
        :rtype: int
        """
        if len(grid) == 0 or len(grid[0]) == 0:
            return 0

        rows = len(grid)
        cols = len(grid[0])
        visited = [[0] * cols for _ in range(rows)]
        print(visited)
        islands = 0
        for i in range(rows):
            for j in range(cols):
                if grid[i][j] == '0' or visited[i][j] == 1:
                    continue
                visit(grid, visited, i, j)
                islands += 1
        return islands

def visit(grid, visited, i, j):
    rows, cols = len(grid), len(grid[0])
    if i < 0 or i >= rows or j < 0 or j >= cols or grid[i][j] == '0' or visited[i][j] == 1:
        return
    visited[i][j] = 1
    visit(grid, visited, i, j - 1)
    visit(grid, visited, i, j + 1)
    visit(grid, visited, i - 1, j)
    visit(grid, visited, i + 1, j)
