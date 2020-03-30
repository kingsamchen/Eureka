#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 令 dp[i,j] 表示如果位置 (i,j) 能够组成符合题意的方块，方块的最长边长
// 1) 初始化整个 dp 数组为0，同时对于dp[0,j] & dp[i,0] 在遍历时处理，不要预处理，因为最大结果可能出现在最外两边
// 2) 如果 arr[i,j] == '0'，则肯定不能组成方块，直接考虑下一个
// 3) 否则，dp[i,j] = min(dp[i-1,j-1], dp[i-1,j], dp[i,j-1]) + 1，即考虑当前方块左、上，对角三个点各自
//    为方块右下角的情况
// 因为面积最大值不一定出现在 dp[-1,-1]，即最后一个点能够组成方块也不代表它是最大面积方块的一部分（可能单独自己）
// 所以需要额外维护一个 max_area
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        if (matrix.empty()) {
            return 0;
        }

        int rows = matrix.size();
        int cols = matrix.front().size();
        vector<vector<int>> dp(rows, vector<int>(cols));

        int max_len = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (i == 0 || j == 0) {
                    // This could also affect our final result.
                    dp[i][j] = matrix[i][j] - '0';
                } else if (matrix[i][j] == '1') {
                    dp[i][j] = min({dp[i-1][j-1], dp[i-1][j], dp[i][j-1]}) + 1;
                }

                max_len = max(max_len, dp[i][j]);
            }
        }

        return max_len * max_len;
    }
};
