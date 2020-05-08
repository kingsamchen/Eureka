#include <numeric>
#include <vector>

using namespace std;

// 核心思路
// 转换为背包问题：令 dp[i,s] = true 表示前i个元素存在子集可以组成和为s
// 那么最终结果就是 dp[n, sum/2]
// 另外因为奇数不可能被均分，所以如果一开始 sum/2 不是偶数就可以提前结束
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        // Cannot be odd
        if (sum % 2) {
            return false;
        }

        vector<vector<bool>> dp(nums.size() + 1, vector<bool>(sum / 2 + 1));
        for (int i = 1; i <= nums.size(); ++i) {
            dp[i][0] = true;
        }

        for (int i = 1; i <= nums.size(); ++i) {
            for (int s = 1; s <= sum / 2; ++s) {
                if (s < nums[i - 1]) {
                    dp[i][s] = dp[i-1][s];
                } else {
                    dp[i][s] = dp[i-1][s] || dp[i-1][s-nums[i-1]];
                }
            }
        }

        return dp.back().back();
    }
};
