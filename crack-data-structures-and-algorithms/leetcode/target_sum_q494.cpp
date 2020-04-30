#include <unordered_map>
#include <vector>

using namespace std;

// 核心思路
// 采用回溯遍历 + memo；memo 这里要注意对 start 分层：不同 start 可能会有相同的 sum，相互之间不能混淆
// 所以核心是 <start, sum> -> ways；具体到实现可以不同
// base结束条件：start == |nums| 遍历结束，此时 sum == 0 代表找到一组解，否则没找到
// 如果 memo 里找不到 <start, sum> 对应的结果，需要分治计算：因为当前值可以取+/-，所以两个分支都要考虑
// 递归处理
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int S) {
        vector<unordered_map<int, int>> memo(nums.size());
        return count_ways(memo, 0, nums, S);
    }

    int count_ways(vector<unordered_map<int, int>>& memo, int start, vector<int>& nums, int64_t sum)
    {
        if (start == nums.size()) {
            return sum == 0 ? 1 : 0;
        }

        auto it = memo[start].find(sum);
        if (it != memo[start].end()) {
            return it->second;
        }

        auto cnt1 = count_ways(memo, start + 1, nums, sum - nums[start]);
        auto cnt2 = count_ways(memo, start + 1, nums, sum + nums[start]);
        memo[start][sum] = cnt1 + cnt2;
        return cnt1 + cnt2;
    }
};
