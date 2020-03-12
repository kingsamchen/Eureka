#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

// 核心思路
// 维护两个变量：
// 一个是维护目前最大子数组和的 max_sum
// 另一个是维护目前大于0的连续和 accum（max_sum 的候选）
// 如果加上某个元素后 accum < 0 那么需要将 accum 置零并从下一个元素重新开始
// 因为两点：
// 1) 如果下一个元素 >= 0，我们直接从下一个元素开始算和反而更大
// 2) 如果下一个元素 < 0，只会让 accum 变得更小
// 所以综上 accum >= 0
// 另外注意如果不使用INT_MIN哨兵值，则需要从第二个元素开始遍历，同时
// max_sum = nums[0] && accum = max(nums[0], 0)
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int max_sum = nums[0];
        int accum = max(nums[0], 0);
        for (auto it = next(nums.begin()); it != nums.end(); ++it) {
            accum += *it;
            max_sum = max(max_sum, accum);
            if (accum < 0) {
                accum = 0;
            }
        }

        return max_sum;
    }
};
