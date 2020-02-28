// 核心思路
// 本质是 fast pow，不过因为n有可能是负数，所以当n为奇数时需要多考虑一种情况
class Solution {
public:
    double myPow(double x, int n) {
        if (n == 0) {
            return 1.;
        }

        if (x == 1.) {
            return 1.;
        }

        auto ans = myPow(x * x, n / 2);
        if (n % 2 == 0) {
            return ans;
        }

        if (n > 0) {
            return ans * x;
        } else {
            return ans / x;
        }
    }
};
