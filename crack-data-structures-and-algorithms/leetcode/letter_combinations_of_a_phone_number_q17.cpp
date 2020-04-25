#include <string>
#include <string_view>
#include <vector>

using namespace std;

// 核心思路
// 回溯遍历：对于当前digit对应的字母集合逐一生成一个串，然后递归处理下一个digit
// 生成下一个字母的串前需要把上一个字母去掉（对应 L35~L36）
// 递归结束条件为整个digits完整遍历结束

constexpr string_view kTable[] = {
    "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
};

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) {
            return {};
        }

        vector<string> result;
        string combi;
        gen_combination(digits, 0, combi, result);
        return result;
    }

    void gen_combination(const string& digits, int pos, string& combi, vector<string>& result) {
        if (pos == digits.size()) {
            result.push_back(combi);
            return;
        }

        auto idx = digits[pos] - '0' - 2;
        for (char ch : kTable[idx]) {
            combi += ch;
            gen_combination(digits, pos + 1, combi, result);
            combi.pop_back();
        }

        return;
    }
};
