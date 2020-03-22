#include <vector>

using namespace std;

// 核心思路
// 贪心策略为：每次下一跳选择能够跳的距离最远的点
// 即，假设当前位置为i，允许最大步数是nums[i]，那么当前位置可以跳的最远的距离是i+nums[i]
// 那么对于 i + 1 <= j <= i + nums[i]，我们选择令 j + nums[j] 最大的j作为下一跳
// 结束条件为：如果当前最远的距离覆盖最后一个元素，那么下一跳就可以结束
// 注意：|nums| <= 1 直接退出即可，因为不需要跳
class Solution {
public:
    int jump(vector<int>& nums) {
        if (nums.size() <= 1) {
            return 0;
        }

        int jmps = 0;
        auto farest = nums[0];
        for (int i = 0; farest < nums.size() - 1;) {
            auto next_pos = 0;
            auto next_farest = 0;
            for (auto j = i + 1; j <= farest; ++j) {
                if (nums[j] + j > next_farest) {
                    next_farest = nums[j] + j;
                    next_pos = j;
                }
            }

            i = next_pos;
            farest = next_farest;
            ++jmps;
        }

        ++jmps;

        return jmps;
    }
};
