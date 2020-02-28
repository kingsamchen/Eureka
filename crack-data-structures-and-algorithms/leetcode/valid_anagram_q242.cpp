#include <string>

using namespace std;

// 核心思路
// 首先需要两个字符串长度一致
// 然后用一个辅助数组，记录字符串s每个字符出现的次数；然后检查t的字符出现次数是不是和s一致
// 因为我们首先保证了字符串长度一致，所以不会出现t是s的字串这种情况
class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.size() != t.size()) {
            return false;
        }

        int table[26] {};
        for (auto ch : s) {
            ++table[ch - 'a'];
        }

        for (auto ch : t) {
            if (table[ch - 'a']-- == 0) {
                return false;
            }
        }

        return true;
    }
};
