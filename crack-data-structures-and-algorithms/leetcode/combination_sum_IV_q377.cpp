#include <algorithm>
#include <unordered_map>
#include <vector>

using namespace std;

// 核心思路
// 令 dp[i] 表示 target == i 时的可能组合总数，那么我们有
// dp[i] = sigma k dp[i-k]，其中 k 是输入数组 nums 的任一元素，且满足 k <= i
// 输入数组 nums 需要排序以便于性能微调
// follow up: 如果输入包含负数则可能出现长度无限的组合，所以这种情况需要限制序列长度
class Solution {
public:
    int combinationSum4(vector<int>& nums, int target) {
        vector<unsigned> dp(target + 1, 0);
        dp[0] = 1;

        sort(nums.begin(), nums.end());

        for (int i = 1; i <= target; ++i) {
            for (auto val : nums) {
                if (val > i) {
                    break;
                }
                dp[i] += dp[i-val];
            }
        }

        return dp.back();
    }
};

// 核心思路
// 回溯+memo
// 回溯的时候从数组的第一个元素开始尝试
class Solution2 {
public:
    int combinationSum4(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        unordered_map<int, int> memo;
        return calc_combi(nums, target, 0, memo);
    }

    int calc_combi(vector<int>& nums, int target, int start, unordered_map<int, int>& memo) {
        if (target == 0) {
            return 1;
        }

        auto it = memo.find(target);
        if (it != memo.end()) {
            return it->second;
        }

        int ways = 0;
        for (auto i = start; i < nums.size(); ++i) {
            auto delta = target - nums[i];
            if (delta < 0) {
                break;
            }

            ways += calc_combi(nums, delta, 0, memo);
        }

        memo[target] = ways;

        return ways;
    }
};
