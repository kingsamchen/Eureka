#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 令 dp[i][j] 为从起点（第一行）开始到 (i,j) 的 minimum falling path sum，那么则有
// dp[0][j] = A[0][j], 并且
// dp[i][j] = min(dp[i-1][j], dp[i-1][j-1], dp[i-1][j+1]) + A[i][j], 1 <= j < cols - 1
// 对于 j == 0 or j == cols - 1，只能考虑一边，这点需要注意
// 最后的结果出现在 min(dp[-1].begin(), dp[-1].end()) 中
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& A) {
        int dp[100][100];

        int rows = A.size();
        int cols = A[0].size();

        for (auto i = 0; i < cols; ++i) {
            dp[0][i] = A[0][i];
        }

        for (int i = 1; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                auto prev = dp[i-1][j];
                if (j - 1 >= 0) {
                    prev = min(prev, dp[i-1][j-1]);
                }
                if (j + 1 < cols) {
                    prev = min(prev, dp[i-1][j+1]);
                }
                dp[i][j] = prev + A[i][j];
            }
        }

        return *min_element(&dp[rows-1][0], &dp[rows-1][cols]);
    }
};
