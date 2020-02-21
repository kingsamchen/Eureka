#include <vector>

using namespace std;

// 核心思路
// 因为数组已经排序，这里使用双指针法，分别用l、r从左右两边扫描元素
// 如果 a[l] + a[r] > target，那么 r 需要往左递减；a[l] + a[r] < target，那么 l 需要递增往右
// 题目已经规定了一定有且只有一个解
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int l = 0;
        int r = numbers.size() - 1;
        while (l < r) {
            auto sum = numbers[l] + numbers[r];
            if (sum > target) {
                --r;
            } else if (sum < target) {
                ++l;
            } else {
                return {l+1, r+1};
            }
        }

        return {};
    }
};