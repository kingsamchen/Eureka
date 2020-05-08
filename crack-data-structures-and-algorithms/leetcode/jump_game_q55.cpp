#include <vector>

using namespace std;

// 核心思路
// 题目中最大可以跳K步可以转换成：每次前进一步，最多前进K步，但是可以在任意一步停下选择续命
// 我们用 steps 表示当前还剩下的跳跃长度，那么开始的每轮循环
// 0) 循环开始代表当前位置是下一跳的出发点，我们可以选择使用当前位置nums[i]来替代steps
//    或者保持steps不变
// 1) 如果 steps < 0 则说明上一跳后其实已经结束了，我们甚至没法用当前位置的步数来续命
// 2) 否则用贪心策略检查是否要更新剩余步数，即如果 steps < nums[i] 则更新 steps = nums[i]，续命
// 3) 不管是否续命steps都要前进，所以 --steps
// 注意，这个算法很好处理了 nums = [0] 的情况
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int steps = 0;
        for (auto num : nums) {
            // 上一跳已经结束了，无法开始下一跳
            if (steps < 0) {
                return false;
            }

            // 续命
            if (steps < num) {
                steps = num;
            }

            // 不管是否续命都要前进一步
            --steps;
        }

        return true;
    }
};
