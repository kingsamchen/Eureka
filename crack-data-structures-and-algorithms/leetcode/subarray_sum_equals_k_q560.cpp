#include <unordered_map>
#include <vector>

using namespace std;

// 核心思路
// 题目要求的就是求有多少对 (i,j), i <= j 使得 sum[i...j] == K
// sum[i...j] 可以转换为 sum[0...j] - sum[0...i)，即(i, j) 对应的数组累计和
// 于是我们有：对于一个给定的下标j，有多少个下标i, i < j 使得 sum[0...j] - K = sum[0...i) 成立
// 再 arr[0...j] 里就有多少个连续子数组的和等于K。
// 因为我们需要的不是 sum[0...i) 的和，而是这个和对应的个数，所以我们用一个 hash-table 记录 sum -> cnt
// 关于初始化 0 -> 1 这个 pair
// 因为前面的逻辑 arr[0...j] 范围内的个数依赖于 arr[0...j) 的个数，我们没有考虑 sum[0...j] == K 的情况
// 所以要么单独加上这个判断，要么直接把 sum[0...j] - K = 0 这种 case 提前初始化为1
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> accum_table {{0, 1}};
        int sum = 0;
        int cnt = 0;
        for (auto val : nums) {
            sum += val;
            auto it = accum_table.find(sum - k);
            cnt += it == accum_table.end() ? 0 : it->second;
            // No need once we have initialized accum_table with (0, 1) pair.
            // if (sum == k) {
            //     ++cnt;
            // }
            ++accum_table[sum];
        }

        return cnt;
    }
};
