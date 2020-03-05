#include <string>
#include <vector>

using namespace std;

// 核心思路
// 这题直接回溯会超时，需要使用DP
// dp[i] = dp[i-1] + dp[i-2], if s[i] != 0 && s[i-1] != 0 && s[i-1...i]是合法组合
// 细分case就是：
// 1) 只要 s[i] != 0，dp[i] 就依赖 dp[i-1]
// 2) 只要 s[i-1...i]是一个合法组合，dp[i] 就依赖 dp[i-2]；因为这里需要 i > 1
//    所以 dp[1] 需要特殊判断
class Solution {
public:
    int numDecodings(string s) {
        vector<int> dp(s.size());
        dp[0] = s[0] != '0' ? 1 : 0;
        for (auto i = 1; i < s.size(); ++i) {
            auto extra = 0;
            if (s[i-1] != '0' && stoi(s.substr(i-1, 2)) <= 26) {
                extra = i > 1 ? dp[i-2] : 1;
            }

            if (s[i] == '0') {
                dp[i] = extra;
            } else {
                dp[i] = dp[i-1] + extra;
            }
        }

        return dp.back();
    }
};
