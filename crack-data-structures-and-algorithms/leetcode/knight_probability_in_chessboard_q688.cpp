#include <utility>
#include <vector>

using namespace std;

// 核心思路
// 令 dp[r,c,k] 为当前位于 (r, c) 且能够移动 k 次后仍在棋盘的概率
// 我们有 dp[r,c,0] = 1 对任何棋盘的坐标 (r, c)
// 并且递推方程 dp[r,c,k] = sigma dp[r',c',k-1], (r', c') 为任何棋盘上的合法位置并且可以经过一次移动
// 到 (r, c) 的位置
// 这里用递归 & memo的实现是可以避免计算很多用不到的结果，因为每次 decision making 的结果很多，但是状态转移
// 时的情况只有1-2种
class Solution {
public:
    double knightProbability(int N, int K, int r, int c) {
        double dp[25][25][101] {};
        for (int i = 0; i < 25; ++i) {
            for (int j = 0; j < 25; ++j) {
                dp[i][j][0] = 1;
            }
        }

        return calc_prob(N, K, r, c, dp);
    }

    double calc_prob(int N, int K, int r, int c, double dp[25][25][101]) {
        if (dp[r][c][K] != 0.) {
            return dp[r][c][K];
        }

        auto poses = next_positions(N, r, c);
        double prob = 0.;
        for (const auto& pos : poses) {
            prob += calc_prob(N, K - 1, pos.first, pos.second, dp) / 8;
        }

        dp[r][c][K] = prob;

        return prob;
    }

    vector<pair<int, int>> next_positions(int N, int r, int c) {
        vector<pair<int, int>> positions;

        auto rp = {-2, -1, 1, 2};
        for (int p : rp) {
            auto rn = r + p;
            auto d = 3 - abs(p);
            auto cn1 = c + d;
            auto cn2 = c - d;
            if (0 <= rn && rn < N) {
                if (0 <= cn1 && cn1 < N) {
                    positions.emplace_back(rn, cn1);
                }

                if (0 <= cn2 && cn2 < N) {
                    positions.emplace_back(rn, cn2);
                }
            }
        }

        return positions;
    }
};
