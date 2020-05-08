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

// O(nlogn) 分治法
// 将数组划分为 arr[l...m...r)
// ma为arr[l...m)的最大子数组和；mb为arr[m...r)最大子数组和
// 如果原数组的最大子数组和出现在连接左右两个数组的中间，即包含arr[m]，则这部分记为 mc
// 所以mc是以m为起点，往左右两边扩张的最大子数组和
// 原数组解为 max(ma, mb, mc)
class Solution2 {
public:
    int maxSubArray(vector<int>& nums) {
        return max_sub(nums, 0, nums.size());
    }

    int max_sub(const vector<int>& nums, int first, int last) {
        if (first >= last) {
            return 0;
        }

        if (first + 1 == last) {
            return nums[first];
        }

        auto mid = first + (last - first) / 2;

        int tmp_max = nums[mid];
        int left_max = tmp_max;
        for (int i = mid - 1; i >= first; --i) {
            tmp_max += nums[i];
            left_max = max(left_max, tmp_max);
        }

        tmp_max = nums[mid];
        int right_max = tmp_max;
        for (int i = mid + 1; i < last; ++i) {
            tmp_max += nums[i];
            right_max = max(right_max, tmp_max);
        }

        auto mc = left_max + right_max - nums[mid];
        auto ma = max_sub(nums, first, mid);
        auto mb = max_sub(nums, mid, last);

        return max({mc, ma, mb});
    }
};
