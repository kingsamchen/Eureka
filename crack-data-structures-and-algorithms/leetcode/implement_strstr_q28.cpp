#include <string>

using namespace std;

// 核心思路
// no cheating!
// 首先判断绝对不可能的case，开头两个if
// 然后双层循环，外层遍历haystack，内层遍历needle
// 外层每迭代一次，内层要从头开始遍历；另外小心比较时候i+j不要越界
class Solution {
public:
    int strStr(string haystack, string needle) {
        if (needle.empty()) {
            return 0;
        }

        if (haystack.size() < needle.size()) {
            return -1;
        }

        for (auto i = 0; i < haystack.size(); ++i) {
            auto j = 0;
            for (; j < needle.size(); ++j) {
                if (i + j >= haystack.size()) {
                    return -1;
                }

                if (haystack[i+j] != needle[j]) {
                    break;
                }
            }
            if (j == needle.size()) {
                return i;
            }
        }

        return -1;
    }
};
