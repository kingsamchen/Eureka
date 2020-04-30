#include <string>

using namespace std;

// 核心思路
// 首先判断绝对不可能的case，开头两个if
// 然后双层循环：外层遍历haystack，每次选择一个新的起点进行匹配；内层遍历needle，检查当前是否能够匹配上
// 内层核心是检查 haystack[p] == needle[j] 是否满足；但是别忘了首先保证p不越界
// 外层每迭代一次，内层要从头开始遍历
class Solution {
public:
    int strStr(string haystack, string needle) {
        if (needle.empty()) {
            return 0;
        }

        if (haystack.empty()) {
            return -1;
        }

        for (int i = 0; i + needle.size() <= haystack.size(); ++i) {
            int p = i;
            int j = 0;
            for (; j < needle.size(); ++j, ++p) {
                if (p == haystack.size()) {
                    return -1;
                }

                if (haystack[p] != needle[j]) {
                    break;
                }
            }

            // all matched.
            if (j == needle.size()) {
                return i;
            }
        }

        return -1;
    }
};
