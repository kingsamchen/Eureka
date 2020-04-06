#include <string>
#include <utility>

using namespace std;

// 核心思路
// 首先定义字符表，包含每个基础数字表示
// 然后对给定的 num 从对照表的最大数值开始向下遍历
// 1) 如果 num == 0 说明结束
// 2) 如果 num < table[i].val 说明 num 小于当前对照字符，继续向下遍历
// 3) 否则 (num >= table[i].val) num 的罗马表示里包含当前字符；num -= table[i].val 后继续
//    从当前字符开始检查，因为可能可以重复用当前罗马数字表示
const pair<int, string> kTable[] = {
    {1, "I"},
    {4, "IV"},
    {5, "V"},
    {9, "IX"},
    {10, "X"},
    {40, "XL"},
    {50, "L"},
    {90, "XC"},
    {100, "C"},
    {400, "CD"},
    {500, "D"},
    {900, "CM"},
    {1000, "M"}
};

constexpr size_t kSize = 13;

class Solution {
public:
    string intToRoman(int num) {
        string roman;
        for (int i = kSize - 1; i >= 0;) {
            if (num == 0) {
                break;
            }

            if (num < kTable[i].first) {
                --i;
                continue;
            }

            roman += kTable[i].second;
            num -= kTable[i].first;
        }

        return roman;
    }
};