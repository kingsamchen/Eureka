#include <string>

using namespace std;

// 核心思路
// 框架和 Q2: Add Two Numbers 一模一样
// 逆序遍历两个字符串，分别对每位做加法，这里需要先字符->数字
// 如果一个字符串已经遍历结束，那么后序遍历一直返回0；遍历终止条件是 while p1 >= 0 || p2 >= 0
// 最后别忘了结束得首要单独判断一下 carry 是否有值；然后 reverse 结果即可
class Solution {
public:
    string addStrings(string num1, string num2) {
        int p1 = num1.size() - 1;
        int p2 = num2.size() - 1;
        int carry = 0;
        string ans;
        while (p1 >= 0 || p2 >= 0) {
            int val1 = p1 >= 0 ? num1[p1] - '0' : 0;
            int val2 = p2 >= 0 ? num2[p2] - '0' : 0;
            auto sum = val1 + val2 + carry;

            carry = sum / 10;
            sum %= 10;
            ans.append(1, '0' + sum);

            if (p1 >= 0) {
                --p1;
            }

            if (p2 >= 0) {
                --p2;
            }
        }

        if (carry) {
            ans.append(1, '0' + carry);
        }

        reverse(ans.begin(), ans.end());

        return ans;
    }
};
