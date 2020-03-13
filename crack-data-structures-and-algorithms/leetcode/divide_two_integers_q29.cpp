#include <algorithm>
#include <cstdint>

using namespace std;

// 核心思路
// 先把两个数都求int64_t下的绝对值，分别为 m 和 n
// 1) 如果 m < n，那么最后结果是0
// 2) 因为不能用乘除法，所以这里用另外一种方式实现逼近
//    因为 m >= n，所以我们可以用方法 m = 2^t * n + k的方式来快速逼近。
//    我们令 factor = 2^t * n，round 来计数 2^t；初始条件下，t=0，即 round=1 factor=n
//    只要 m > factor * 2就可以继续操作；否则继续对 m - factor, n 递归处理
class Solution {
public:
    int divide(int dividend, int divisor) {
        // Cast to int64_t to avoid handling INT_MIN & INT_MAX
        auto result = do_div(abs(static_cast<int64_t>(dividend)), abs(static_cast<int64_t>(divisor)));
        if ((dividend >= 0) ^ (divisor >= 0)) {
            result = -result;
        }

        if (result > INT_MAX || result < INT_MIN) {
            result = INT_MAX;
        }

        return static_cast<int>(result);
    }

    int64_t do_div(int64_t m, int64_t n) {
        if (m < n) {
            return 0;
        }

        int64_t factor = n, round = 1;
        while (m > (factor << 1)) {
            factor <<= 1;
            round <<= 1;
        }

        return round + do_div(m - factor, n);
    }
};
