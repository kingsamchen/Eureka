#include <functional>
#include <vector>

using namespace std;

// 核心思路
// 直接使用 lower_bound & upper_bound 解决
// 注意这里复用一套框架，抽象了 predicate
// 另注意这里的 predicate 和 STL 里用的 predicate 语义作用都不一样
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        auto lb = search_bound(nums, target, [](int val, int target) { return val >= target; });
        auto ub = search_bound(nums, target, [](int val, int target) { return val > target; });
        return lb == nums.size() || nums[lb] != target ? vector<int>{-1, -1} : vector<int>{lb, ub - 1};
    }

    int search_bound(const vector<int>& nums, int target, function<bool(int val, int target)> pred) {
        int count = nums.size();
        int first = 0;
        while (count > 0) {
            int half = count / 2;
            int mid = first + half;
            if (!pred(nums[mid], target)) {
                first = mid + 1;
                count -= half + 1;
            } else {
                count = half;
            }
        }

        return first;
    }
};
