#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 令 dp[i] 代表从i-th step走到顶的最小代价
// 因为每次可以选择往前走1步或者2步，因此倒数第一步和倒数第二步的代价是确定的
// 反向推出状态方程
// dp[i] = min(dp[i+1], dp[i+2]) + cost[i], i in [0, len-2)
// dp[len-1] = cost[len-1] && dp[len-2] = cost[len-2]
// 注意结果值可能出现在 dp[0] 或 dp[1]，这也是为什么题目保证 |cost| >= 2
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int len = cost.size();
        vector<int> dp(len);
        dp[len - 1] = cost[len - 1];
        dp[len - 2] = cost[len - 2];
        for (auto i = len - 3; i >= 0; --i) {
            dp[i] = min(dp[i+1], dp[i+2]) + cost[i];
        }

        return min(dp[0], dp[1]);
    }
};
