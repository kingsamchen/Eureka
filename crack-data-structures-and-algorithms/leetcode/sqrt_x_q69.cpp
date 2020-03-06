// 核心思路
// 维护 [l, r) 区间，二分探测
// 因为OJ要在数范围上坑人，所以取中和探测都做了处理
class Solution {
public:
    int mySqrt(int x) {
        if (x <= 1) {
            return x;
        }

        auto l = 1, r = x;
        while (l + 1 < r) {
            auto m = l + (r - l) / 2;
            auto quo = x / m;
            if (quo == m) {
                return m;
            } else if (quo < m) {
                r = m;
            } else {
                l = m;
            }
        }

        return l;
    }
};
