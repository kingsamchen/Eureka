#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 令 dp[i][j] 代表走到 (i,j) 的最小路径和，则我们有
// dp[0][0] = grid[0][0]
// dp[i][0] = dp[i-1][0] + grid[i][0], 1 <= i < rows
// dp[0][j] = dp[0][j-1] + grid[0][j], 1 <= j < cols
// dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + grid[i][j], 1 <= i, j < rows/cols
// 然后根据状态方程求解即可
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();

        vector<vector<int>> dp(rows, vector<int>(cols));
        dp[0][0] = grid[0][0];

        for (auto i = 1; i < rows; ++i) {
            dp[i][0] = dp[i-1][0] + grid[i][0];
        }

        for (auto i = 1; i < cols; ++i) {
            dp[0][i] = dp[0][i-1] + grid[0][i];
        }

        for (auto i = 1; i < rows; ++i) {
            for (auto j = 1; j < cols; ++j) {
                dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + grid[i][j];
            }
        }

        return dp.back().back();
    }
};
