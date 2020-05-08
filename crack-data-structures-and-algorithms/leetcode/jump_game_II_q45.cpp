#include <vector>

using namespace std;

// 核心思路
// 贪心策略为：每次下一跳选择能够跳的距离最远的点
// 即，假设当前位置为i，允许最大步数是nums[i]，那么当前位置可以跳的最远的距离是i+nums[i]
// 1) 如果 i+nums[i] >= |nums| 则说明当前位置一跳可以直接结束，不用再仔细选择下一跳 (L19 ~ L21)
// 2) 否则利用贪心策略仔细选择下一跳：我们计算以i位起点，跳j步后再下一次能跳的最大距离
//    即，1 <= j <= steps，选择令 j + nums[i+j] 最大的j为跳跃步数，i+j为下一跳的位置 (L26 ~ L35)
// 注意：|nums| <= 1 这种情况可以不用特殊判断完美解决
class Solution {
public:
    int jump(vector<int>& nums) {
        int jmps = 0;
        int steps = 0;
        for (int i = 0; i < nums.size() - 1;) {
            // 当前可跳最大步数
            steps = nums[i];
            // 当前位置覆盖全程只需要一跳就结束
            if (i + steps >= nums.size() - 1) {
                ++jmps;
                break;
            }

            int max_dist = 0;
            int next_idx;
            for (int j = 1; j <= steps; ++j) {
                auto idx = i + j;
                auto dist = nums[idx] + j;
                if (dist > max_dist) {
                    max_dist = dist;
                    next_idx = idx;
                }
            }

            // 设定下一跳位置
            i = next_idx;
            ++jmps;
        }

        return jmps;
    }
};
