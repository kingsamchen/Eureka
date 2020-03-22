#include <vector>

using namespace std;

// 核心思路
// 核心是判断失败和贪心的续命步数
// 我们用 steps 表示当前还剩下的跳跃长度，那么开始的每轮循环里
// 1) 如果 steps < 0 则说明上一跳后其实已经结束了，我们甚至没法用当前位置的步数来续命
// 2) 否则用贪心策略检查是否要更新剩余步数，即如果 steps < nums[i] 则更新 steps = nums[i]，续命
// 3) 当前元素到此已经处理结束，接下来 steps 自减表明往前进一步
// 注意，这个算法很好处理了 nums = [0] 的情况
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int steps = 0;
        for (auto num : nums) {
            if (steps < 0) {
                return false;
            }

            // longer jump length, update.
            if (steps < num) {
                steps = num;
            }

            --steps;
        }

        return true;
    }
};
