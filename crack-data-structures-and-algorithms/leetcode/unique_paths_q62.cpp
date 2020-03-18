// 核心思路
// 用 dp[i][j] 表示从起点到 (i, j) 的唯一路径数，那么则有
// dp[0][j] = dp[i][0] = 1 以及
// dp[i][j] = dp[i-1][j] + dp[i][j-1]
class Solution {
public:
    int uniquePaths(int m, int n) {
        // 1 <= m, n <= 100
        int dp[100][100] {0};

        dp[0][0] = 1;

        for (auto i = 1; i < m; ++i) {
            dp[i][0] = 1;
        }

        for (auto i = 1; i < n; ++i) {
            dp[0][i] = 1;
        }

        for (auto i = 1; i < m; ++i) {
            for (auto j = 1; j < n; ++j) {
                dp[i][j] = dp[i-1][j] + dp[i][j-1];
            }
        }

        return dp[m-1][n-1];
    }
};
