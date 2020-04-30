#include <vector>

using namespace std;

// 核心思路
// 令 dp[k,i] 表示k次投掷最后一次点数为i的次数；易知有 dp[1,i]=1, 0 <= i < 6 （这里直接下标从0开始）
// 并且 dp[k,i] = (sigma(j) dp[k-1,j]) - illegal , 0 <= j < 6, 1 <= k <= N
// illegal 的计算要相对麻烦一点
// 注：k代表当前第k-th投掷的结果，所以k-1就是前面**已经完成**的k-1次投掷
// 1) 如果 k - 1 == max_roll[j]，即当前第一次超出最大连续允许次数，那么 ways -= 1
// 2) 如果 k - 1 > max_roll[j] && i != j（最大连续次数限制这里不可能相等），则 ways -= dp[k-1-max_roll[j]]
// 举例：点数1最大连续次数是4，那么
// k - 1 < 4 => 不用考虑限制
// k - 1 == 4 => 1 1 1 1 需要去掉前四个都是1的情况
// k - 1 == 5 => X 1 1 1 1 这里 X 点数是为2~6，即排除自身为1的可能 dp[k-1-4,X] 1 <= X < 6
// k - 1 == 6 => Y X 1 1 1 1 需要去掉 dp[k-1-4,X] 的结果
// 注：这题因为大数溢出取模的问题，额外的坑不少
class Solution {
public:
    int dieSimulator(int n, vector<int>& rollMax) {
        constexpr int kMod = 1000000007;
        int dp[5001][6] {};

        for (int i = 0; i < 6; ++i) {
            dp[1][i] = 1;
        }

        for (int k = 2; k <= n; ++k) {
            for (int i = 0; i < 6; ++i) {
                int ways = 0;
                for (int j = 0; j < 6; ++j) {
                    ways = (ways + dp[k - 1][j]) % kMod;
                }
                auto max_roll = rollMax[i];
                if (k - 1 == max_roll) {
                    --ways;
                } else if (k - 1 > max_roll) {
                    for (int t = 0; t < 6; ++t) {
                        if (t != i) {
                            ways = (ways - dp[k-1-max_roll][t] + kMod) % kMod;
                        }
                    }
                }

                dp[k][i] = ways;
            }
        }

        int total = 0;
        for (int i = 0; i < 6; ++i) {
            total = (dp[n][i] + total) % kMod;
        }

        return total;
    }
};
