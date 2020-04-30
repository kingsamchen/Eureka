#include <vector>

using namespace std;

// 核心思路
// 1) 首先将二维棋盘压缩为0~9的一维，这样就不用按照点坐标来计算了；同时直接预处理好每个数对应的下一个点的列表
// 即 `next_moves` 列表
// 2) 然后令 dp[k,i] 为初始位置在 i 并且生成长度为 k 的不同序列的个数，则有
// dp[k,i] = sigma(p, dp[k-1,p]) p为从i出发可达的位置；易知
// dp[1,i] = 1，0 <= i <= 9
// 3) 最后的总数就是 sigma(p, dp[N,p])
class Solution {
public:
    int knightDialer(int N) {
        constexpr int kMod = 1000000007;

        int dp[5001][10] {};
        for (int i = 0; i < 10; ++i) {
            dp[1][i] = 1;
        }

        vector<vector<int>> next_moves {{4, 6}, {6, 8}, {7, 9}, {4, 8}, {3, 9, 0}, {}, {1, 7, 0}, {2, 6}, {1, 9}, {4, 2}};

        for (int move = 2; move <= N; ++move) {
            for (int i = 0; i < 10; ++i) {
                for (int p : next_moves[i]) {
                    dp[move][i] = (dp[move-1][p] + dp[move][i]) % kMod;
                }
            }
        }

        int total = 0;
        for (int i = 0; i < 10; ++i) {
            total = (total + dp[N][i]) % kMod;
        }

        return total;
    }
};
