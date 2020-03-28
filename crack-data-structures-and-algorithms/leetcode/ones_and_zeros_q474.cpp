#include <algorithm>
#include <string>
#include <vector>

using namespace std;

// 核心思路
// 本质是背包问题，但是为了便于处理需要降维（将第i个字符串处理这个维度消掉）
// 降维之后状态方程需要从后往前推，因此令 dp[i,j] 为使用m个0，n个1时能够表示的最大字符串数，
// 则有，对于每一个字符串，dp[i,j] = max(dp[i,j], dp[i-zc][j-oc] + 1), 第一个参数表示不使用当前字符串
// dp数组需要初始化为全0
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int dp[101][101] {};
        for (const auto& s : strs) {
            int zc, oc;
            zc = count(s.begin(), s.end(), '0');
            oc = s.size() - zc;
            for (int i = m; i >= zc; --i) {
                for (int j = n; j >= oc; --j) {
                    dp[i][j] = max(dp[i][j], dp[i-zc][j-oc] + 1);
                }
            }
        }

        return dp[m][n];
    }
};
