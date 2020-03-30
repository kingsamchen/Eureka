#include <algorithm>
#include <cstdint>

// 核心思路
// 为了处理 int32 的范围问题，转换成 int64_t
// 并且不需要特别处理负数，直接常规/10以及%10分离出每一位即可
class Solution {
public:
    int reverse(int x) {
        int64_t val = static_cast<int64_t>(x);
        int64_t result = 0;
        while (val != 0) {
            auto m = val % 10;
            val /= 10;
            result = result * 10 + m;
        }

        return INT_MIN <= result && result <= INT_MAX ? result : 0;
    }
};
