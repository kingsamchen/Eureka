// 核心思路
// fast pow，本质是减少乘法次数，这里每次递归n的规模少一半，但是只用了一次乘法
// 不过因为n有可能是负数，所以当n为奇数时需要多考虑一种情况
class Solution {
public:
    double myPow(double x, int n) {
        if (x == 1.0) {
            return 1.0;
        }

        if (n == 0) {
            return 1.0;
        }

        if (n == 1) {
            return x;
        }

        if (n == -1) {
            return 1 / x;
        }

        auto t = myPow(x, n / 2);
        auto p = t * t;
        if (n % 2) {
            if (n < 0) {
                p /= x;
            } else {
                p *= x;
            }
        }

        return p;
    }
};
