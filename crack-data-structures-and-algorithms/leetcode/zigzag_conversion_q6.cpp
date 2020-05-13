#include <string>

using namespace std;

// 核心思路
// 并不需要真的生成旋转之后的字符串，找规律生成最终结果即可
// 假设题目要求r行, 0 ~ r-1，那么第一行和最后一行距离下一列的序号插值时 2r - 2
// 即第一行第一列的字符序号是0，那么这一行第二列需要是 0 + 2r - 2，以此类推；最后一行一样
// 1th ~ r-2th存在中间多一列，中间那列的序号是 i+2r-2-2*i
// 必要时在纸上画画就可以好到规律
class Solution {
public:
    string convert(string s, int numRows) {
        if (numRows < 2) {
            return s;
        }

        string result;
        for (int i = 0; i < numRows; ++i) {
            for (int j = i;; j += 2 * numRows - 2) {
                if (i != 0 && i != numRows - 1) {
                    auto r = j - 2 * i;
                    if (r < s.size()) {
                        result.push_back(s[r]);
                    }
                }

                if (j >= s.size()) {
                    break;
                }

                result.push_back(s[j]);
            }
        }

        return result;
    }
};
