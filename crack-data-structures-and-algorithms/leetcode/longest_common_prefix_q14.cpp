#include <vector>
#include <string>

using namespace std;

// 核心思路
// 第i轮循环遍历每个字符串i-th字符检查是否匹配
// 如果有不匹配，则直接返回目前最长的匹配；否则增加匹配长度
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) {
            return "";
        }

        int length = 0;
        for (int i = 0;; ++i) {
            char ch = 0;
            for (const string& s : strs) {
                if (i >= s.size()) {
                    return strs[0].substr(0, length);
                }

                if (ch == 0) {
                    ch = s[i];
                } else {
                    if (ch != s[i]) {
                        return strs[0].substr(0, length);
                    }
                }
            }
            ++length;
        }
    }
};
