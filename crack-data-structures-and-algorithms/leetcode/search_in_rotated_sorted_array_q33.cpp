#include <vector>

using namespace std;

// 核心思路
// 1) 基于二分查找，所以命中元素的情况这里是不变的，nums[m] == t
// 2) 减治的部分，首先在纸上画出两种翻转的示意图，即 nums[l] < nums[m] 和 nums[l] > nums[m]
// 3) 对于每种翻转的情况，缩小区间的核心点是，选择单调的那部分判断，简单不易出错
//    a) 对于 nums[l] < nums[m] 这种情况，单调的部分是 nums[l...m]，所以对应判断t在这个分区
//       nums[l] <= t && t < nums[m]
//    b) 类似的对于 nums[l] > nums[m] 这种情况，单调的部分在 nums[m...r)，对应判断t是否在这个分区
//       nums[m] < t && t < nums[l] 因为nums[r]不可访问所以这用nums[l]来检查
// 4) 根据具体情况缩减区间，保证 nums[l, r) 仍是一个 rotated sorted array 即可
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int l = 0;
        int r = nums.size();
        while (l < r) {
            auto m = l + (r - l) / 2;
            // 找到元素的情况
            if (nums[m] == target) {
                return m;
            }

            // 分图像case；因为范围是 [l, r) 所以只能用nums[l]作为检测点
            if (nums[l] < nums[m]) {
                if (nums[l] <= target && target < nums[m]) {
                    r = m;
                } else {
                    l = m + 1;
                }
            } else {
                if (nums[m] < target && target < nums[l]) {
                    l = m + 1;
                } else {
                    r = m;
                }
            }
        }

        return -1;
    }
};
