#include <unordered_map>
#include <vector>

using namespace std;

// 核心思路
// 使用hash-table作为辅助结构，保存 <number, index>
// 题目已经规定了只有一组解
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> table;
        for (auto i = 0; i < nums.size(); ++i) {
            table[nums[i]] = i;
        }

        for (auto i = 0; i < nums.size(); ++i) {
            auto delta = target - nums[i];
            auto it = table.find(delta);
            if (it != table.cend() && it->second != i) {
                return {i, it->second};
            }
        }

        return {};
    }
};
