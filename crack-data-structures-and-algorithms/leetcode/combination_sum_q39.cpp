#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// DFS回溯：
// 1) 每个元素都可能存在/不存在当前的一组解，所以需要逐个遍历分别尝试
// 2) 如果选择 candidates[i]，那么将问题归约到 target - candidates[i]，并且因为此题元素可以重复选取，所以
//    子问题里仍然可以从 i 开始选
//    注意：子问题是从i开始选，而不是从头(start)；因为candidates[start...i)范围内的元素都已经处理过了
//    每个 combi_sum() 调用接受的 candidates 实质上是 candidates[start:] 第一个子集
// 3) 提前对 candidates 排序可以优化 target - val < 0 的剪枝情况
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
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
            auto val = candidates[i];
            if (target - val < 0) {
                // The return statement here require candidates being sorted.
                return;
            }

            combi.push_back(val);
            combi_sum(candidates, i, target - val, combi, result);
            combi.pop_back();
        }
    }
};
