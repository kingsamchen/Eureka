#include <vector>

using namespace std;

// 核心思路：
// 本质是计算矩阵里的连通区域（标记为1）；经典的 DFS 应用
// 直接用 DFS 遍历一块完整区域；用 visited 辅助矩阵记录已经遍历过的区域
// 最后遍历的区域数就是要求的结果
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid.front().empty()) {
            return 0;
        }

        int rows = grid.size();
        int cols = grid.front().size();

        vector<vector<bool>> visited(rows, vector<bool>(cols));
        int island_cnt = 0;
        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                if (grid[i][j] == '0' || visited[i][j]) {
                    continue;
                }

                dfs_visit(grid, visited, i, j);
                ++island_cnt;
            }
        }

        return island_cnt;
    }

    void dfs_visit(const vector<vector<char>>& grid, vector<vector<bool>>& visited, int i, int j) {
        int rows = grid.size();
        int cols = grid.front().size();

        if (i < 0 || i >= rows || j < 0 || j >= cols || grid[i][j] == '0' || visited[i][j]) {
            return;
        }

        visited[i][j] = true;
        dfs_visit(grid, visited, i, j - 1);
        dfs_visit(grid, visited, i - 1, j);
        dfs_visit(grid, visited, i, j + 1);
        dfs_visit(grid, visited, i + 1, j);
    }
};
