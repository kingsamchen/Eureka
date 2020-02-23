#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 题目只要求和的个数，并不要求下标组合，所以沿用 3Sum 的思路框架
// 不一样是最内层循环的处理
// 如果 sum < target，则因为数组是有序的，[l, r]范围内共r-l个组合都是符合要求，可以直接求得；然后自增l，尝试下一组
// 如果 sum >= target，说明我们的和要变小，所以自减r，尝试下一组
// 因为每次判断l或者r都会移动，所以最内层仍然是O(n)，整体保持O(n^2)
class Solution {
public:
    int threeSumSmaller(vector<int>& nums, int target) {
        if (nums.size() < 3) {
            return 0;
        }

        sort(nums.begin(), nums.end());
        int count = 0;
        for (auto i = 0; i < nums.size() - 2; ++i) {
            auto l = i + 1;
            auto r = nums.size() - 1;
            while (l < r) {
                auto sum = nums[i] + nums[l] + nums[r];
                if (sum >= target) {
                    --r;
                } else {
                    count += r - l;
                    ++l;
                }
            }
        }

        return count;
    }
};
