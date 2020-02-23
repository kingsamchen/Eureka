#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 和 3sum 类似，都是先排序然后分内外层处理
// 这里题目要求的是最接近目标和三数之和，所以固定好外层之后扫描内层，不断更新min_dist和closest
// 注意：可能出现sum和target相等的情况下，这种case直接返回
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        if (nums.size() < 3) {
            return 0;
        }

        sort(nums.begin(), nums.end());
        int closest = 0;
        int min_dist = INT_MAX;
        for (auto i = 0; i < nums.size() - 2; ++i) {
            auto l = i + 1;
            auto r = nums.size() - 1;
            while (l < r) {
                int sum = nums[i] + nums[l] + nums[r];
                int diff = sum - target;
                if (diff == 0) {
                    return sum;
                }

                if (abs(diff) < min_dist) {
                    closest = sum;
                    min_dist = abs(diff);
                }

                if (diff > 0) {
                    --r;
                } else {
                    ++l;
                }
            }
        }

        return closest;
    }
};