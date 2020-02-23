#include <string>

using namespace std;

// 核心思路
// 因为题目说只考虑alphanumeric字符，所以开一个新的字符串只存符合的字符
// 然后就是简单的左右双指针扫描
class Solution {
public:
    bool isPalindrome(string s) {
        string clean;
        for (auto ch : s) {
            if (isalnum(ch)) {
                clean += tolower(ch);
            }
        }

        int l = 0;
        int r = clean.size() - 1;
        while (l < r) {
            if (clean[l++] != clean[r--]) {
                return false;
            }
        }

        return true;
    }
};
