#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 和I的两点区别：1) candidates 包含重复整数 2) 每个候选元素只能使用一次
// 对于 (2) 每个候选元素只能使用一次，只需要在处理归约子问题时令 start = i + 1，子问题的候选输入就不会包含已经使用的元素
// 区别 (1) 同样会导致结果包含重复，考虑输入 1 1 2 5 并且 target = 8：如果第一个元素的1能够组成 (1, 2, 5) 一组解
// 那么第二个元素 1 也会生成这组解，导致重复
// 因此为了避免重复，已经处理完一个候选之后，检查 candidates[i] != candidates[i-1] 是否满足；如果不满足说明重复，跳过当前元素即可
// 注意这里的前提条件是 candidates[i-1] 这个元素已经被我们处理过，这个由 i != start 保证
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());

        vector<int> combi;
        vector<vector<int>> result;
        combi_sum(candidates, 0, target, combi, result);

        return result;
    }

    void combi_sum(const vector<int>& candidates, int start, int target, vector<int>& combi, vector<vector<int>>& result) {
        if (target == 0) {
            result.push_back(combi);
            return;
        }

        for (auto i = start; i < candidates.size(); ++i) {
            if (i != start && candidates[i] == candidates[i-1]) {
                continue;
            }

            auto val = candidates[i];
            if (target - val < 0) {
                return;
            }

            combi.push_back(val);
            combi_sum(candidates, i + 1, target - val, combi, result);
            combi.pop_back();
        }
    }
};
