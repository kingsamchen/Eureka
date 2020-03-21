#include <algorithm>
#include <bitset>
#include <vector>

using namespace std;

// 核心思路
// 用 dp[i] 表示第 i 天的最小旅游花销，那么则有
// 1) 如果 i 不在 days 里（不出游），那么 dp[i] = dp[i-1]，否则
// 2) dp[i] = min(dp[i-1] + costs[0], dp[max(0,i-7)] + costs[1], dp[max(0,i-30)] + costs[2])
// dp[0] = 0 因为不出游；同时不满7天/30天的差值可能为负数的，直接从第1天开始考虑，即使用 dp[0]
// 注意不能用 INT_MAX 这种哨兵值，因为有可能日票价格比周票、月票还高
class Solution {
public:
    int mincostTickets(vector<int>& days, vector<int>& costs) {
        int dp[366] {0};

        bitset<366> dayset;
        for (auto d : days) {
            dayset.set(d);
        }

        for (auto i = days[0]; i <= days.back(); ++i) {
            if (!dayset.test(i)) {
                dp[i] = dp[i-1];
                continue;
            }

            int d1 = dp[i-1] + costs[0];
            int d7 = dp[max(0, i - 7)] + costs[1];
            int d30 = dp[max(0, i - 30)] + costs[2];
            dp[i] = min({d1, d7, d30});
        }

        return dp[days.back()];
    }
};
