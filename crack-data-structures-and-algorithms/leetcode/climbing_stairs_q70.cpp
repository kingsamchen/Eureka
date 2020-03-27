#include <vector>

using namespace std;

// 核心思路
// 令 dp[i] 为爬到阶梯 i 的不同种方式，则我们有
// dp[i] = dp[i-1] + dp[i-2]，i >= 3
// dp[1] = 1, dp[2] = 2
class Solution {
public:
    int climbStairs(int n) {
        if (n == 1) {
            return 1;
        }

        if (n == 2) {
            return 2;
        }

        vector<int> dp(n + 1);
        dp[1] = 1;
        dp[2] = 2;
        for (int i = 3; i <= n; ++i) {
            dp[i] = dp[i-1] + dp[i-2];
        }

        return dp[n];
    }
};
