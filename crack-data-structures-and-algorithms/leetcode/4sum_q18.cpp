#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 整体逻辑和 3Sum 一模一样，只是多了一层（一个数字），并且要求的和是不固定的target。
// 总之，对于非最内层来说，这一层有k个数字（固定一个之后还剩k-1个），那么遍历范围就是 j in [i+1, |nums|-1-k)，
// i是上一层的游标
// 重复判断的逻辑就是 j > i + 1 && nums[j] == nums[j-1]
// 理解重复处理：上一个元素下我已经得到了一组解，本次元素如果和上一个元素一样，那么重复，需要跳过；立足点是我们已经
// 有了一个解。
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        if (nums.size() < 4) {
            return {};
        }

        sort(nums.begin(), nums.end());
        vector<vector<int>> result;

        for (auto i = 0; i < nums.size() - 3; ++i) {
            if (i > 0 && nums[i] == nums[i-1]) {
                continue;
            }

            for (auto j = i + 1; j < nums.size() - 2; ++j) {
                if (j > i + 1 && nums[j] == nums[j-1]) {
                    continue;
                }

                auto l = j + 1;
                auto r = nums.size() - 1;
                while (l < r) {
                    auto sum = nums[i] + nums[j] + nums[l] + nums[r];
                    if (sum < target) {
                        ++l;
                    } else if (sum > target) {
                        --r;
                    } else {
                        result.push_back({nums[i], nums[j], nums[l], nums[r]});

                        do {
                            ++l;
                        } while (l < r && nums[l] == nums[l-1]);

                        do {
                            --r;
                        } while (l < r && nums[r] == nums[r+1]);
                    }
                }
            }
        }

        return result;
    }
};
