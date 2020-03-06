#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// dp[i]表示已nums[i]结尾的数组的LIS长度，所以我们有
// dp[i] = dp[j] + 1, for 0 <= j < i and nums[i] > nums[j]
// otherwise, dp[i] = 1
// 复杂度 O(n^2)
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }

        vector<int> dp(nums.size(), 1);
        int max_len = 1;
        for (auto i = 1; i < nums.size(); ++i) {
            for (int j = i - 1; j >= 0; --j) {
                if (nums[i] > nums[j]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }

            max_len = max(max_len, dp[i]);
        }

        return max_len;
    }
};

// Followup: O(nlogn) 做法

// 核心思路
// 使用一个 bookkeeping 数组保存可能的LIS
// 因为这个数组是递增的，所以可以用二分查找
// 如果能在bk数组中找到当前元素val的lower_bound，就更新；如果找不到，就追加当前val
// 注：bk数组保存的不一定是合法的LIS，但是长度可以保持一致
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> keeping;
        for (auto val : nums) {
            auto it = lower_bound(keeping.begin(), keeping.end(), val);
            if (it == keeping.end()) {
                keeping.push_back(val);
            } else {
                *it = val;
            }
        }

        return keeping.size();
    }
};
