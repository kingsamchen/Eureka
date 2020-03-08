#include <string>
#include <unordered_map>

using namespace std;

// 核心思路
// 统计每个字符出现次数然后再次遍历找到第一个只出现一次的
class Solution {
public:
    int firstUniqChar(string s) {
        unordered_map<char, int> table;
        for (auto ch : s) {
            ++table[ch];
        }

        for (int i = 0; i < s.size(); ++i) {
            if (table[s[i]] == 1) {
                return i;
            }
        }

        return -1;
    }
};
