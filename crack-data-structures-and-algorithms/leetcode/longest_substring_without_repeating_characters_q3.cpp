#include <algorithm>
#include <string>
#include <unordered_map>

using namespace std;

// 核心思路
// 采用 sliding window，window 内为不包含重复字符的子串
// 子串长度就是窗口大小
// left 来维护窗口的起始边界
// 同时利用 hash-table 维护每个字符最近一次对应的下标；每次窗口缩小都通过下标
// 来维护新窗口的 left 起始
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> table;
        int max_len = 0;
        auto l = 0;
        for (auto r = 0; r < s.size(); ++r) {
            auto ch = s[r];
            auto it = table.find(ch);
            if (it == table.end() || it->second < l) {
                table[ch] = r;
                max_len = max(max_len, r - l + 1);
            } else {
                l = it->second + 1;
                it->second = r;
            }
        }

        return max_len;
    }
};
