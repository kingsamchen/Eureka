// 核心思路
// 简单的DP应用
// dp[n] = dp[n-1] + dp[n-2]
class Solution {
public:
    int climbStairs(int n) {
        if (n == 1) {
            return 1;
        }

        if (n == 2) {
            return 2;
        }

        int base1 = 1, base2 = 2;
        for (auto i = 3; i <= n; ++i) {
            auto new_base = base1 + base2;
            base1 = base2;
            base2 = new_base;
        }

        return base2;
    }
};
