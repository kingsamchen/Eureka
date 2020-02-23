#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 题目要求的是数字组合而不是下标，所以可以排序，数组有序是处理最内层两个数的前提条件。
// 1) 首先固定最外层的数字，遍历范围是 [0, |nums|-2)
//    同时要考虑避免重复，所以当最外层数字**至少**是第二轮时，就要检测当前数字是否和前一个相同
//    如果相同就换下一个，直到找到一个不同的
// 2) 三个数的情况只有两层，所以第二层就是最内层，直接用双指针法来检测元素和
//    遍历范围是 [i+1, |nums|)，其中i是外层元素的位置，这里就是第一层
//    最内层同样也要考虑重复的情况：当元素和满足题目要求时，分别对l和r进行相同元素跳过处理
//    注意要先各自移位再进入判断逻辑，因为我们要先**结束**当前case，下一个case才能判断是否和上一个相同
//    判断时别忘了 l < r 的前置要求
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        // Precondition.
        if (nums.size() < 3) {
            return {};
        }

        vector<vector<int>> result;
        sort(nums.begin(), nums.end());

        for (auto i = 0; i < nums.size() - 2; ++i) {
            if (i > 0 && nums[i] == nums[i-1]) {
                continue;
            }

            auto l = i + 1;
            auto r = nums.size() - 1;
            while (l < r) {
                auto sum = nums[i] + nums[l] + nums[r];
                if (sum > 0) {
                    --r;
                } else if (sum < 0) {
                    ++l;
                } else {
                    result.push_back({nums[i], nums[l], nums[r]});

                    do {
                        ++l;
                    } while (l < r && nums[l] == nums[l-1]);

                    do {
                        --r;
                    } while (l < r && nums[r] == nums[r+1]);
                }
            }
        }

        return result;
    }
};
