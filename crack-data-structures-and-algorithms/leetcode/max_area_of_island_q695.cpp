#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 解题框架和 q200 number of islands 这道一模一样：
// 如果 grid[i][j] == 1 && not visited[i][j]，那么将这个点加入当前岛屿的面积，标记后以这个点为起始点
// 按照 DFS 的方式对外扩散
// visited 数组是用来防止一个岛屿点被多次遍历
// 注：遍历判断条件不满足提前退出可以让代码逻辑更清晰
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        if (grid.empty() || grid.front().empty()) {
            return 0;
        }

        int rows = grid.size();
        int cols = grid.front().size();

        vector<vector<bool>> visited(rows, vector<bool>(cols));

        int max_area = 0;
        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                if (grid[i][j] == 0 || visited[i][j]) {
                    continue;
                }

                int area = 0;
                dfs_visit(grid, visited, i, j, area);
                max_area = max(max_area, area);
            }
        }

        return max_area;
    }

    void dfs_visit(const vector<vector<int>>& grid, vector<vector<bool>>& visited, int i, int j, int& area) {
        int rows = grid.size();
        int cols = grid.front().size();

        if (i < 0 || i >= rows || j < 0 || j >= cols || grid[i][j] == 0 || visited[i][j]) {
            return;
        }

        ++area;
        visited[i][j] = true;

        dfs_visit(grid, visited, i, j - 1, area);
        dfs_visit(grid, visited, i - 1, j, area);
        dfs_visit(grid, visited, i, j + 1, area);
        dfs_visit(grid, visited, i + 1, j, area);
    }
};
