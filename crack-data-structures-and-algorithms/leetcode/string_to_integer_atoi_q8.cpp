#include <algorithm>
#include <cstdint>
#include <string>

using namespace std;

// 核心思路
// 1) 跳过开头的空格
// 2) 处理前缀-/+
// 3) 如果 pos < |str| 并且str[pos]是数字，则拼到 val 里
//    为了能够处理溢出，这里使用 int64 存储结果
//    考虑到 int64 仍然有可能溢出，所以每次处理完一位就检查当前数字是否大于 INT32_MAX，超过就提前中止
//    这里也能很好地处理str=INT_MIN的情况，因为 abs(INT_MIN) > INT_MAX，循环退出，val=INT_MIN 等于结果
class Solution {
public:
    int myAtoi(string str) {
        bool sign = false;
        int64_t val = 0;

        auto pos = str.find_first_not_of(' ');
        if (pos == string::npos) {
            return 0;
        }

        if (str[pos] == '-') {
            sign = true;
            ++pos;
        } else if (str[pos] == '+') {
            ++pos;
        }

        for (; pos < str.size() && isdigit(str[pos]); ++pos) {
            auto digit = str[pos] - '0';
            val = val * 10 + digit;
            // This even handles INT_MIN well.
            if (val > INT_MAX) {
                break;
            }
        }

        val = sign ? -val : val;

        val = min<int64_t>(max<int64_t>(val, INT_MIN), INT_MAX);

        return val;
    }
};
