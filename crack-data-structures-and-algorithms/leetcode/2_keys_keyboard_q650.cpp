#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 令 dp[i] 为长度为 i 时的最短操作步数，我们有 dp[1] = 0, dp[2] = 2
// 并且因为每次都只能全部复制进行粘贴，所以长度i一定由它的因数粘贴而来，所以
// dp[i] = i，如果 i 为素数
// dp[i] = min(dp[i], dp[j] + (i - j) / j + 1); 对每个因数 j
class Solution {
public:
    int minSteps(int n) {
        int dp[1001] {};
        for (int i = 2; i <= n; ++i) {
            dp[i] = i;
            for (int j = i - 1; j > 1; --j) {
                if (i % j == 0) {
                    dp[i] = min(dp[i], dp[j] + (i - j) / j + 1);
                }
            }
        }

        return dp[n];
    }
};

// 另外一种解法，本质相同，但是直接把 n 的因数列表求出
// 然后对因数列表的每个元素进行 dp 递归求解

class Solution2 {
public:
    int minSteps(int n) {
        vector<int> dp(1001, -1);
        dp[1] = 0;
        dp[2] = 2;
        dp[3] = 3;
        min_steps(n, dp);
        return dp[n];
    }

    void min_steps(int n, vector<int>& dp) {
        if (n <= 3) {
            return;
        }

        auto divisors = extract_divisors(n);
        int min_sts = INT_MAX;
        for (auto d : divisors) {
            if (dp[d] == -1) {
                min_steps(d, dp);
            }
            min_sts = min(min_sts, dp[d] + (n - d) / d + 1);
        }
        dp[n] = min_sts;
    }

    vector<int> extract_divisors(int n) {
        vector<int> divisors {1};
        auto x = static_cast<int>(ceil(sqrt(n)));
        for (int i = 2; i <= x; ++i) {
            if (n % i == 0) {
                divisors.push_back(i);
            }
        }

        for (int i = divisors.size() - 1; i > 0; --i) {
            auto d = n / divisors[i];
            if (d > divisors.back()) {
                divisors.push_back(d);
            }
        }

        return divisors;
    }
};
