#include <string>
#include <unordered_map>

using namespace std;

// 核心思路
// 采用 Look-one-ahead 策略，如果当前罗马数字小于下一个，则说明结果要减去当前数字，即
// 如果 i < len - 1 && roman(s[i]) < roman(s[i+1]) 则 val -= roman(s[i])
// 否则 val += roman(s[i])
class Solution {
public:
    int romanToInt(string s) {
        const unordered_map<char, int> kTable = {
            {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}
        };

        int val = 0;
        for (auto i = 0; i < s.size(); ++i) {
            if (i < s.size() - 1 && kTable.at(s[i]) < kTable.at(s[i+1])) {
                val -= kTable.at(s[i]);
            } else {
                val += kTable.at(s[i]);
            }
        }

        return val;
    }
};
