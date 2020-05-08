#include <string>

using namespace std;

// 核心思路
// 大体上采取列竖式相乘的做法，因此首先保证 |num1| >= |num2|
// 1) 因为竖式是从右往左计算，为计算方便，结果保存时选择逆序保存，即低位在头
// 2) 两重循环，num2 的数字遍历在外层，num1 遍历在里层，按照列竖式计算
//    假设结果的数字要保存到result[dst]，则我们有
//    product = d2 * d1 + carry + result[dst], dst 如果不存在说明result需要扩充补0
//    carry = product / 10; product %= 10; result[dst] = product
// 3) dst = i + j 其中i代表num2已经处理了i位，j代表num1处理了j位；因为num2在外层，所以i这里起base的作用
//    因为num2每处理完一位，结果存储的时候就会往高位进一位
// 4) num2每处理一位，carry要清零，这意味着上一位结尾的时候要把留下来的carry处理掉
//    因为两个数相乘的和不可能超过99，所以carry不可能超过9，只需要处理一位
class Solution {
public:
    string multiply(string num1, string num2) {
        if (num1 == "0" || num2 == "0") {
            return "0";
        }

        string digits;

        if (num1.size() > num2.size()) {
            swap(num1, num2);
        }

        for (int i = 0; i < num2.size(); ++i) {
            int d2 = num2[num2.size() - 1 - i] - '0';
            int carry = 0;
            for (int j = 0; j < num1.size(); ++j) {
                int d1 = num1[num1.size() - 1 - j] - '0';

                auto dst = i + j;
                if (dst >= digits.size()) {
                    digits.push_back('0');
                }

                auto product = d1 * d2 + carry + (digits[dst] - '0');
                carry = product / 10;
                product %= 10;

                digits[dst] = product + '0';
            }

            if (carry > 0) {
                digits.push_back(carry + '0');
            }
        }

        reverse(digits.begin(), digits.end());

        return digits;
    }
};
