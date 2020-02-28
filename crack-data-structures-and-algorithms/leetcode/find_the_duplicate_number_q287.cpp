#include <vector>

using namespace std;

// 核心思路
// n+1个数落在 [1, n] 区间内
// 每一轮算出中位数 m = (1+n) / 2
// 然后遍历数组统计 a[i] <= m 的元素个数；因为求中位数默认用的floor，所以计数包含等于中位数的
// 假设 n 个数落在 [1, n] 区间内，则小于等于m的数的个数恰好是m自身；所以
// 如果 cnt <= m，则说面 [1, m] 范围内不存在重复元素，我们的范围变成 [m+1, n]
// 反之，如果 cnt > m，则说明重复元素就在 [1, m] 范围内
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int low = 1, up = nums.size() - 1;
        while (low < up) {
            auto median = (low + up) / 2;
            int lcnt = 0;
            for (auto val : nums) {
                if (val <= median) {
                    ++lcnt;
                }
            }

            if (lcnt <= median) {
                low = median + 1;
            } else {
                up = median;
            }
        }

        return low;
    }
};
