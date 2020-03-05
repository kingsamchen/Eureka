#include <algorithm>
#include <unordered_map>
#include <vector>

using namespace std;

// 核心思路
// 首先利用map统计每个元素和他的出现次数 <val, cnt>
// 然后把这个统计结果转换到目标结构里，这里选择了vector，记得 pair 要变成 <cnt, val>
// 然后递减排序（可以用partial_sort())
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int> result;

        unordered_map<int, int> freq;
        for (auto val : nums) {
            freq[val]++;
        }

        vector<pair<int, int>> records;
        for (const auto& p : freq) {
            // <cnt, val>
            records.emplace_back(p.second, p.first);
        }

        // Could use partial_sort() here.
        // Or, simply replace vector with priority_queue.
        sort(records.begin(), records.end(), greater<pair<int,int>>());

        for (auto i = 0; i < k; ++i) {
            result.push_back(records[i].second);
        }

        return result;
    }
};
