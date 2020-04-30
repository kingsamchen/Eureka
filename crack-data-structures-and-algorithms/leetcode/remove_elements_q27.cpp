#include <vector>

using namespace std;

// 核心思路
// nums[0...p)维护不重复的元素
// 这里针对 i == p 做了一个小优化，减少赋值次数
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int p = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] != val) {
                if (p != i) {
                    nums[p] = nums[i];
                }
                ++p;
            }
        }

        return p;
    }
};
