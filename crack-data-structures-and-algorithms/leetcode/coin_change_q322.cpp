#include <algorithm>
#include <cstdint>
#include <vector>

using namespace std;

// 核心思路
// 对于给定的 coins 列表，我们有
// dp[n] = min(dp[n-coins[i]] + 1, dp[n]), 0 <= i < |coins|
// dp[0] = 0
// 因为反复用到了 dp[n] 的的值，所以需要仔细考虑 dp 数组的初始值/哨兵值
// 这个值可以用 amount + 1 或者 -1 来承担
// 如果使用-1，那么min()比较时注意一定要转换成 unsigned 来比较，否则最小值一直都会是-1
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, -1);
        dp[0] = 0;
        for (int i = 1; i <= amount; ++i) {
            for (auto c : coins) {
                if (c <= i && dp[i-c] != -1) {
                    dp[i] = min<uint32_t>(dp[i], dp[i-c] + 1);
                }
            }
        }

        return dp[amount];
    }
};
