#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 令 dp[i] 表示正整数i能用最少的完全平方数之和表示，则
// 所有小于n的完全平方数i有dp[i]=1
// 对非完全平方数i，我们有 dp[i] = 1 + min(dp[i-p0], dp[i-p1], ..., dp[i-pn]), p为小于i的完全平方数
class Solution {
public:
    int numSquares(int n) {
        vector<int> dp(n+1);

        for (int i = 1;; ++i) {
            auto sq = i * i;
            if (sq > n) {
                break;
            }
            dp[sq] = 1;
        }

        for (int i = 2; i <= n; ++i) {
            if (dp[i] == 1) {
                continue;
            }

            int min_cnt = INT_MAX;
            for (int j = 1;; ++j) {
                auto sq = j * j;
                if (sq > i) {
                    break;
                }
                min_cnt = min(min_cnt, dp[i-sq]);
            }

            dp[i] = min_cnt + 1;
        }

        return dp[n];
    }
};
