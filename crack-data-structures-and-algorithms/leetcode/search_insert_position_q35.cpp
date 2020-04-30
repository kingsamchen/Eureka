#include <vector>

using namespace std;

// 核心思路
// 虽然题目保证没有重复，这里用 lower_bound 完全可以直接解决
// lower_bound 细节参考 q34.
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        return lower_bound(nums, target);
    }

    int lower_bound(const vector<int>& nums, int target) {
        int count = nums.size();
        int first = 0;
        while (count > 0) {
            int half = count / 2;
            int mid = first + half;
            if (nums[mid] < target) {
                count -= half + 1;
                first = mid + 1;
            } else {
                count = half;
            }
        }

        return first;
    }
};
