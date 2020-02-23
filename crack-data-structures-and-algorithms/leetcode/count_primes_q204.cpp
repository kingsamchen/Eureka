#include <vector>

using namespace std;

// 核心思路
// 使用筛子法：首先建立一个范围为[0, n)的bool数组（题目要求小于n），记为prime
// prime[i] == true 表明数字i是素数；数组默认所有元素初始化为true
// 从第一个素数2开始，把素数的每个倍数对应标记为false，因为是合数。
// 每次找到下一个素数，然后重复进行倍数标记；最后能取出来的素数个数就是<n的素数个数
class Solution {
public:
    int countPrimes(int n) {
        int count = 0;
        vector<bool> prime(n, true);
        for (auto i = 2; i < n; ++i) {
            if (!prime[i]) {
                continue;
            }

            ++count;

            for (auto j = 2; i * j < n; ++j) {
                prime[i*j] = false;
            }
        }

        return count;
    }
};
