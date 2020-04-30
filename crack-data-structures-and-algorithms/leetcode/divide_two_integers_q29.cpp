#include <algorithm>
#include <cstdint>

using namespace std;

// 核心思路
// 需要一个核心递归处理函数 do_div 输入输出都为 uint64_t 保证后面位运算不会出问题
// 1) 如果 m < n，那么结果是0
// 2) 因为题目对操作限定，这里只能用位移来快速逼近
//    因为 m = kn + c, 0 <= c < n；而位移只能进行二次幂运算，所以这里对 k 进行拆分，转换得到
//    => m = (2^t + z)n + c
//         = 2^t * n + zn + c
// 3) 令 round = 2^t, f = 2^t * n，初始 t' = 0 => round = 1 && f = n
//    每轮迭代通过位移增加t，即*2增大round和f
// 4) 找到最大的t之后，通过 m - f = m - 2^t * n => zn + c 然后继续递归处理就可以最终得到k
class Solution {
public:
    int divide(int dividend, int divisor) {
        auto m = static_cast<uint64_t>(abs(dividend));
        auto n = static_cast<uint64_t>(abs(divisor));
        auto quo = static_cast<int64_t>(do_div(m, n));

        if ((dividend < 0) ^ (divisor < 0)) {
            quo = -quo;
        }

        if (quo < INT_MIN || quo > INT_MAX) {
            quo = INT_MAX;
        }

        return static_cast<int>(quo);
    }

    uint64_t do_div(uint64_t m, uint64_t n) {
        if (m < n) {
            return 0;
        }

        uint64_t round = 1;
        uint64_t f = n;
        while (m >= (f << 1)) {
            f <<= 1;
            round <<= 1;
        }

        return round + do_div(m - f, n);
    }
};
