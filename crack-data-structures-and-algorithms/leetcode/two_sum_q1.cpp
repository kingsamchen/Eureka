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
