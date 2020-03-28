#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

// 核心思路
// 换个角度思考：将石头分两堆，使得两堆的和最接近，这样两队石头互相粉碎后剩下的重量最少；
// 每堆的和应当越接近 sum / 2，sum 是所有石头重量总和
// 这样将问题转换为0-1背包问题：给定一堆石头和容量为 sum / 2 的包，求背包能装入的最大石头容量
// 令 dp[i][v] 表示第 i 块石头时容量为 v 的包裹最多能装石头的重量；1 <= i <= |stones|, 1 <= v <= sum/2
// 如果 stones[i-1] (第i块石头重量) 太大不能装入容量为v的包裹，那么有 dp[i][v] = dp[i-1][v]
// 否则，我们可以选择是否放入第i块石头，选择放入/不放入哪个包裹重量更大来决定：
// dp[i][v] = max(dp[i-1][v], dp[i-1][v-stones[i-1]] + stones[i-1])
// dp[-1][-1] 是某堆最大重量，所以 sum - 2 * dp[-1][-1] 就是我们的结果
class Solution {
public:
    int lastStoneWeightII(vector<int>& stones) {
        auto sum = accumulate(stones.begin(), stones.end(), 0);
        int volume = sum / 2;
        vector<vector<int>> dp(stones.size() + 1, vector<int>(volume + 1));
        for (int i = 1; i <= stones.size(); ++i) {
            auto weight = stones[i-1];
            for (int v = 1; v <= volume; ++v) {
                if (weight > v) {
                    dp[i][v] = dp[i-1][v];
                } else {
                    dp[i][v] = max(dp[i-1][v], dp[i-1][v-weight] + weight);
                }
            }
        }

        return sum - 2 * dp.back().back();
    }
};

// 在 Solution1 的基础上状态方程压缩一个维度
// 状态方程反方向迁移
class Solution2 {
public:
    int lastStoneWeightII(vector<int>& stones) {
        auto sum = accumulate(stones.begin(), stones.end(), 0);
        int vol = sum / 2;
        vector<int> dp(vol + 1);
        for (auto weight : stones) {
            for (int v = vol; v >= weight; --v) {
                dp[v] = max(dp[v], dp[v-weight] + weight);
            }
        }

        return sum - 2 * dp.back();
    }
};
