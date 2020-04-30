#include <vector>

using namespace std;

// 核心思路
// 用变量p来维护nums[0...p]不包含重复元素
// 每次i前进迭代的时候如果nums[i] == nums[p]说明重复直接跳过继续下一个
// 如果不重复，则需要把nums[i]复制到p下一个位置，维护p的不变量语义
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        // guaranteed no duplicates
        if (nums.size() < 2) {
            return nums.size();
        }

        int p = 0;
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] != nums[p]) {
                nums[++p] = nums[i];
            }
        }

        return p + 1;
    }
};
