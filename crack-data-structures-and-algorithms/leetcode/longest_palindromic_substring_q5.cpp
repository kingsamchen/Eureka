#include <string>
#include <utility>
#include <vector>

using namespace std;

// 核心思路
// 使用DP
// dp[i,j] (i <= j) 表示 s[i,j]是否存在回文，那么则有
// dp[i,j] = 1, if i == j
// dp[i,j] = s[i] == s[j], if i + 1 == j
// dp[i,j] = s[i] == s[j] && dp[i+1,j-1] == 1, if j - i > 1
// 注意：因为第三个递推式里出现dp[i,j]由dp[i+1,j-1]导出的情况，所以i段的代码实现上需要逆序遍历
// 额外再使用一个 range pair 维护距离最长的 (i, j)
class Solution {
public:
    string longestPalindrome(string s) {
        vector<vector<bool>> dp(s.size(), vector<bool>(s.size()));
        pair<int, int> range;
        for (int i = s.size() - 1; i >= 0; --i) {
            for (int j = i; j < s.size(); ++j) {
                if (i == j ) {
                    dp[i][j] = true;
                } else if (i + 1 == j) {
                    dp[i][j] = s[i] == s[j];
                } else {
                    dp[i][j] = s[i] == s[j] && dp[i+1][j-1];
                }

                if (dp[i][j] && j - i > range.second - range.first) {
                    range.first = i;
                    range.second = j;
                }
            }
        }

        return s.substr(range.first, range.second - range.first + 1);
    }
};
