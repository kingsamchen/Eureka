
// 核心思路
// 令 dp[d][n] 为 d 个共有 f 面的骰子能能掷出和为 n 的不同方法数，则我们可以发现
// dp[d][n] = sigma dp[d-1][n-i], 1 <= i <= f；即子问题的不同方法数求和
// 特别的，对于 d == 1时，我们有 dp[1][i] = 1
class Solution {
public:
    int numRollsToTarget(int d, int f, int target) {
        int dp[31][1001] {0};

        for (int i = 1; i <= f; ++i) {
            dp[1][i] = 1;
        }

        for (int di = 2; di <= d; ++di) {
            for (int sum = 1; sum <= target; ++sum) {
                int ways = 0;
                for (int fi = 1; fi <= f; ++fi) {
                    if (sum >= fi) {
                        ways += dp[di-1][sum-fi];
                        ways %= (1000000000 + 7);
                    }
                }
                dp[di][sum] = ways;
            }
        }

        return dp[d][target];
    }
};
