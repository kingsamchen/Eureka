#include <vector>

using namespace std;

// 核心思路
// 题目要求第一个缺的正整数，又因为不能排序不能额外空间，所以只能内部元素交换
// 一个技巧是：将值为t的正数放置在下标为t-1的位置上，即保证 nums[i] == i + 1，因为题目
// 要求的是整数，所以可以直接变换之后作为下标落到目标位置上
// 1) 遍历每一个元素，如果 t = nums[i]，并且满足 0 < t <= |nums| && t != nums[t-1]
//    则说明元素t并没有放在自己应在的位置上，所以交换 nums[i] <-> nums[t-1]
// 2) 注意交换之后 nums[i] 位置上的元素仍然可能需要重新放置，所以这里不能前进，需要重新检查
//    处理一次
// 3) 上面的检查完成之后重新遍历数组，第一个 nums[i] != i + 1 的 i + 1 就是我们要找的数字
// 4) 如果(3)检查找不到，则说明现有数组的数字都是连续递增的，那么第一个缺失的就是 |nums| + 1
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        for (int i = 0; i < nums.size();) {
            auto val = nums[i];
            if (0 < val && val <= nums.size() && nums[val-1] != val) {
                swap(nums[i], nums[val-1]);
                continue;
            }
            ++i;
        }

        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] != i + 1) {
                return i + 1;
            }
        }

        return nums.size() + 1;
    }
};
