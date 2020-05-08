#include <vector>

using namespace std;

// 核心思路
// 这题数组可能包含重复元素，因此虽然大体框架一致，处理细节上有区别
// 1) 之前用 nums[l] 和 nuns[m] 的大小来判断旋转图像，确定单调连续的一端出现在哪个位置
//    这里因为元素重复可能会导致 nums[l] == nums[m]，无法决断
//    这种情况放弃一次减半的目的，改为 ++l 只缩减一个元素，并且这样缩减不改变数组的性质
class Solution {
public:
    bool search(vector<int>& nums, int target) {
        int l = 0, r = nums.size();
        while (l < r) {
            auto m = l + (r - l) / 2;
            if (nums[m] == target) {
                return true;
            }

            if (nums[l] < nums[m]) {
                // left part is monotonic
                if (nums[l] <= target && target < nums[m]) {
                    r = m;
                } else {
                    l = m + 1;
                }
            } else if (nums[l] > nums[m]) {     // right part is monotonic
                if (nums[m] < target && target < nums[l]) {
                    l = m + 1;
                } else {
                    r = m;
                }
            } else {
                // nums[l] == nums[m], can't decide; threfore shrink l by one
                ++l;
            }
        }

        return false;
    }
};
