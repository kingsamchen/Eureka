#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 因为 follow up 部分提到了要用 O(n) 的空间，结合这题单独的情况，我们选择：从底部往上处理
// 令 dp[i,j] 表示 (i, j) 位置的 minimum path sum，则我们有
// dp[i,j] = min(dp[i+1,j], dp[i+1,j+1]) + cost[i,j], 0 <= i <= rows-2, 0 <= j <= cols-of-row - 1
// dp[-1,j] = cost[-1,j]，即最后一行的初始值
// 可以发现每次只依赖后一行，所以行状态可以被压缩掉：dp[j] = min(dp[j], dp[j+1]) + cost[i,j]，i & j 范围不变
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        vector<int> dp = triangle.back();
        for (int i = triangle.size() - 2; i >= 0; --i) {
            for (int j = 0; j < triangle[i].size(); ++j) {
                dp[j] = min(dp[j], dp[j+1]) + triangle[i][j];
            }
        }

        return dp[0];
    }
};
